/***************************************************************************
  qgsquicklayertreemodel.cpp
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquicklayertreemodel.h"

#include <qgslayertreemodel.h>
#include <qgslayertreenode.h>
#include <qgslayertree.h>
#include <qgsvectorlayer.h>
#include <qgslayertreemodellegendnode.h>
#include <qgsproject.h>
#include "qgsvectorlayer.h"

#include <QString>

QgsQuickLayerTreeModel::QgsQuickLayerTreeModel( QObject* parent )
  : QSortFilterProxyModel( parent )
  , mProject(0)
  , mLayerTreeModel(0)
  , mLayerTree(0)
{
  connect( this, &QgsQuickLayerTreeModel::projectChanged, this, &QgsQuickLayerTreeModel::onReadProject );
}

QgsQuickLayerTreeModel::~QgsQuickLayerTreeModel() {
    if (mLayerTree) {
        delete mLayerTree;
    }
    if (mLayerTreeModel) {
        delete mLayerTreeModel;
    }
}

void QgsQuickLayerTreeModel::setProject(QgsProject* project) {
    if (project == mProject)
        return;

    // If we have already something connected, disconnect it!
    if (mProject) {
        disconnect(mProject, 0, this, 0);
    }

    mProject = project;

    // Connect all signals
    if (mProject) {
        connect( mProject, &QgsProject::readProject, this, &QgsQuickLayerTreeModel::onReadProject );
    }

    emit projectChanged();
}

void QgsQuickLayerTreeModel::onReadProject() {
    Q_ASSERT(mProject);
    if (mLayerTree) {
        delete mLayerTree;
    }
    if (mLayerTreeModel) {
        delete mLayerTreeModel;
    }

    mLayerTree = new QgsLayerTree();
    /* TODO !!!!
    for(QgsMapLayer* layer: mProject->layers()) {
        if (layer->isValid() && (layer->type() == QgsMapLayer::VectorLayer) )
            mLayerTree->addLayer(layer);
    }
    */

    mLayerTreeModel = new QgsLayerTreeModel(mLayerTree , this );
    setSourceModel( mLayerTreeModel );

    qDebug() << "QgsQuickLayerTreeModel qgsproject loaded " << mProject->fileName();

    reset();
}

QVariant QgsQuickLayerTreeModel::data( const QModelIndex& index, int role ) const
{
  if (!mLayerTreeModel)
      return QVariant();

  switch ( role )
  {
    case Name:
    {
      QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
        return QVariant( nodeLayer->layer()->name() );
      }
      else
      {
        return QVariant();
      }
    }

    case VectorLayer:
    {
      QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( nodeLayer->layer() );
        return QVariant::fromValue<QgsVectorLayer*>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    default:
      return QSortFilterProxyModel::data( index, role );
  }
}

QHash<int, QByteArray> QgsQuickLayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QSortFilterProxyModel::roleNames();
  roleNames[Name] = "name";
  roleNames[VectorLayer] = "vectorLayer";

  return roleNames;
}

QModelIndex QgsQuickLayerTreeModel::index( int row ) const {
    return QSortFilterProxyModel::index(row, 0);
}
