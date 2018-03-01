/***************************************************************************
  main.cpp
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtDebug>
#include <QQmlError>
#include <QDesktopWidget>
#include <QQmlContext>
#include <qqml.h>

#define STR1(x)  #x
#define STR(x)  STR1(x)

#ifdef ANDROID
#include <QFile>
#include <QDir>
#endif

#ifndef ANDROID
#include <QCommandLineParser>
#include <qgis.h>
#endif

#include "qgsapplication.h"
#include "qgsproject.h"
#include "qgsmaplayer.h"
#include "qgslayertreegroup.h"
#include "qgslayertree.h"
#include "qgsmessagelog.h"
#include "qgsquickutils.h"

static void init_qgis()
{
#ifndef ANDROID
#ifdef QGIS_PREFIX_PATH
  ::setenv("QGIS_PREFIX_PATH", STR(QGIS_PREFIX_PATH), true);
#endif
  if ( ::getenv( "QGIS_PREFIX_PATH" ) == 0 )
  {
    qDebug( "== Must set QGIS_PREFIX_PATH in order to get QGIS Quick module running! ==" );
    //return;
  }
#endif

  QTime t;
  t.start();

#ifdef ANDROID
  QDir myDir( QDir::homePath() );
  myDir.cdUp();
  QString prefixPath = myDir.absolutePath();  // something like: /data/data/org.qgis.quick
  ::setenv( "QGIS_PREFIX_PATH", prefixPath.toUtf8().constData(), true );
#endif

  // if not on Android, QGIS_PREFIX_PATH env variable should have been set already
  QgsApplication::init();
  QgsApplication::initQgis();

#ifdef ANDROID
  // QGIS plugins on Android are in the same path as other libraries
  QgsApplication::setPluginPath( QApplication::applicationDirPath() );

  QString qgisDataSubdir = "qgis-data";
  QDir homeDir = QDir::home();
  QString qgisDataPath = QDir::homePath() + "/" + qgisDataSubdir;
  ::setenv( "QGIS_QUICK_DATA_PATH", qgisDataPath.toUtf8().constData(), true );

  QString assetsBasePath( "assets:" );
  qDebug( "assets base path:  %s", assetsBasePath.toLatin1().data() );

  // make sure all necessary QGIS data are available - unpacked from assets
  //if (!homeDir.exists(qgisDataSubdir)) // do it always on startup - for now
  {
    bool res = homeDir.mkpath( qgisDataSubdir );
    qDebug( "mkpath [%d] %s", res, qgisDataPath.toLatin1().data() );
    QDir qgisDataDir( qgisDataPath );

    QStringList qgisDataFiles;
    qgisDataFiles << "resources/qgis.db" << "resources/srs.db";
    qgisDataFiles << "bg.tif";
    qgisDataFiles << "lines.shp" << "lines.dbf" << "lines.prj" << "lines.shx";
    qgisDataFiles << "points.shp" << "points.dbf" << "points.prj" << "points.shx";
    qgisDataFiles << "polys.shp" << "polys.dbf" << "polys.prj" << "polys.shx";
    qgisDataFiles << "test_project.qgs" << "photo.png";

    foreach ( const QString &dataFile, qgisDataFiles )
    {
      QFile f( assetsBasePath + "/" + qgisDataSubdir + "/" + dataFile );
      QString destFilePath = qgisDataPath + "/" + dataFile;
      QDir destFileDir = QFileInfo( destFilePath ).absoluteDir();
      if ( !destFileDir.exists() )
      {
        bool mkres = qgisDataDir.mkpath( QFileInfo( dataFile ).dir().path() );
        qDebug( "mkpath [%d] %s", mkres, destFileDir.absolutePath().toLatin1().data() );
      }
      if ( !QFile( destFilePath ).exists() )
      {
        bool res = f.copy( destFilePath );
        qDebug( "copying [%d] %s", res, destFilePath.toLatin1().data() );
        // by default the permissions are set as readable for everyone - and not writable!
        res = QFile::setPermissions( destFilePath, QFile::ReadUser | QFile::WriteUser | QFile::ReadOwner | QFile::WriteOwner );
        qDebug( "chmod [%d]", res );
      }
    }
  }

  QgsApplication::setPkgDataPath( qgisDataPath );
#endif

  // make sure the DB exists - otherwise custom projections will be failing
  if ( !QgsApplication::createDatabase() )
    qDebug( "Can't create qgis user DB!!!" );

  qDebug( "qgis_init %f [s]", t.elapsed() / 1000.0 );
}


int main( int argc, char *argv[] )
{
  // 1) Initialize QGIS
  QgsApplication app( argc, argv, true );
  init_qgis();
  qDebug( "QGIS_PREFIX_PATH: %s", ::getenv( "QGIS_PREFIX_PATH" ) );

  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "QGIS" );
  QCoreApplication::setOrganizationDomain( "qgis.org" );
  QCoreApplication::setApplicationName( "QGIS Quick Demo App" );
  QCoreApplication::setApplicationVersion( "1.0" );

  // 2) Load QGIS Project
#ifdef QGIS_QUICK_DATA_PATH
  QString dataDir( STR(QGIS_QUICK_DATA_PATH) );
#else
  QString dataDir( ::getenv( "QGIS_QUICK_DATA_PATH" ) );
#endif
  QString projectFile = dataDir + "/test_project.qgs";
  qDebug() << "project file: " << projectFile;
  QgsProject project;
  bool res = project.read( projectFile );
  Q_ASSERT( res );

  QQmlEngine engine;
  engine.addImportPath( QgsApplication::qmlImportPath() );
  engine.rootContext()->setContextProperty( "__project", &project );
  engine.rootContext()->setContextProperty( "__layers", QVariant::fromValue( project.layerTreeRoot()->layerOrder() ) );

  // Set simulated position for desktop builds
#ifndef ANDROID
  bool use_simulated_position = true;
#else
  bool use_simulated_position = false;
#endif
  engine.rootContext()->setContextProperty( "__use_simulated_position", use_simulated_position );

  QQmlComponent component( &engine, QUrl( "qrc:/main.qml" ) );
  QObject *object = component.create();
  \
  if ( !component.errors().isEmpty() )
  {
    qDebug( "%s", QgsApplication::showSettings().toLocal8Bit().data() );

    qDebug() << "****************************************";
    qDebug() << "*****        QML errors:           *****";
    qDebug() << "****************************************";
    for ( const QQmlError &error : component.errors() )
    {
      qDebug() << "  " << error;
    }
    qDebug() << "****************************************";
    qDebug() << "****************************************";
  }

  if ( object == 0 )
  {
    qDebug() << "FATAL ERROR: unable to create main.qml";
    return EXIT_FAILURE;
  }

#ifndef ANDROID
  QCommandLineParser parser;
  parser.addVersionOption();
  parser.process( app );
#endif

  // Add some data for debugging if needed
  QgsApplication::messageLog()->logMessage( QgsQuickUtils::instance()->dumpScreenInfo() );
  QgsApplication::messageLog()->logMessage( "All up and running" );

  return app.exec();
}

