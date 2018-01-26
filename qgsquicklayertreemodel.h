/***************************************************************************
  qgsquicklayertreemodel.h
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


#ifndef QGSQUICKLAYERTREEMODEL_H
#define QGSQUICKLAYERTREEMODEL_H

#include <QSortFilterProxyModel>

class QgsLayerTree;
class QgsLayerTreeModel;
class QgsProject;

class QgsQuickLayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY( QgsProject* project MEMBER mProject WRITE setProject NOTIFY projectChanged)

  public:
    enum Roles
    {
      Name = Qt::UserRole + 1,
      VectorLayer
    };
    Q_ENUMS( Roles )

    explicit QgsQuickLayerTreeModel(QObject* parent = nullptr );
    ~QgsQuickLayerTreeModel();

    Q_INVOKABLE QVariant data( const QModelIndex& index, int role ) const override;

    Q_INVOKABLE QModelIndex index( int row ) const;

    QHash<int, QByteArray> roleNames() const override;

    void setProject(QgsProject *project);

  signals:
    void projectChanged();

  private slots:
    void onReadProject();

  private:
    QgsProject* mProject;
    QgsLayerTreeModel* mLayerTreeModel;
    QgsLayerTree* mLayerTree;
};

#endif // QGSQUICKLAYERTREEMODEL_H
