/*
 * Copyright (c) 2013 - 2019 Jolla Ltd.
 *
 * All rights reserved.
 *
 * This file is part of Sailfish Transfer Engine package.
 *
 * You may use this file under the terms of the GNU Lesser General
 * Public License version 2.1 as published by the Free Software Foundation
 * and appearing in the file license.lgpl included in the packaging
 * of this file.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file license.lgpl included in the packaging
 * of this file.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

#ifndef TRANSFERENGINE_P_H
#define TRANSFERENGINE_P_H

#include <QObject>
#include <QMap>
#include <QVariantList>

#include "mediatransferinterface.h"
#include "transfermethodinfo.h"

namespace Accounts {
class Manager;
}

class QFileSystemWatcher;
class QTimer;
class QUrl;
class TransferEngine;
class TransferPluginInfo;

class TransferEngineSignalHandler: public QObject
{
    Q_OBJECT
public:
    static TransferEngineSignalHandler * instance();
    static void signalHandler(int signal);

Q_SIGNALS:
    void exitSafely();

private:
    TransferEngineSignalHandler();
};

class ClientActivityMonitor: public QObject
{
    Q_OBJECT
public:
    ClientActivityMonitor(QObject *parent = 0);
    ~ClientActivityMonitor();

    void newActivity(int transferId);
    void activityFinished(int transferId);

    bool activeTransfers() const;
    bool isActiveTransfer(int transferId) const;

public Q_SLOTS:
    void checkActivity();

Q_SIGNALS:
    void transfersExpired(QList<int> transferIds);

private:
    // Map for transferId, timestamps
    QMap<int, quint32> m_activityMap;
    QTimer *m_timer;
};

class TransferEnginePrivate: QObject
{
    Q_OBJECT
public:
    // CallbackElement enum is used as an accessor to DBus callback
    // information from a list. Enums are used as an indeces.
    enum CallbackElements {
        Service = 0,
        Path,
        Interface
    };

    // CallbackMethodType enum is used as an accessor to the list where
    // method names starts from the index 3.
    enum CallbackMethodType {
        CancelCallback  = 3,
        RestartCallback = 4
    };

    TransferEnginePrivate(TransferEngine *parent);
    void recoveryCheck();
    void sendNotification(TransferEngineData::TransferType type,
                          TransferEngineData::TransferStatus status,
                          qreal progress,
                          const QString &fileName,
                          int transferId,
                          bool canCancel,
                          const QUrl &localFileUrl = QUrl());
    int uploadMediaItem(MediaItem *mediaItem,
                        MediaTransferInterface *muif,
                        const QVariantMap &userData);
    inline TransferEngineData::TransferType transferType(int transferId);
    void callbackCall(int transferId, CallbackMethodType method);

public Q_SLOTS:
    void exitSafely();
    void delayedExitSafely();
    void enabledPluginsCheck();
    void cleanupExpiredTransfers(const QList<int> &expiredIds);
    void pluginDirChanged();
    void uploadItemStatusChanged(MediaTransferInterface::TransferStatus status);
    void updateProgress(qreal progress);
    void pluginInfoReady();
    void pluginInfoError(const QString &msg);

public:
    QStringList pluginList() const;
    QList <TransferMethodInfo> enabledPlugins() const;
    QList<QVariantMap> pluginMetaData() const;
    MediaTransferInterface *loadPlugin(const QString &pluginId) const;
    QString mediaFileOrResourceName(MediaItem *mediaItem) const;

private:
    QMap <MediaTransferInterface*, int> m_plugins;
    QMap <int, TransferEngineData::TransferType> m_keyTypeCache;
    bool m_notificationsEnabled;
    QList <TransferPluginInfo*> m_infoObjects;
    QList <TransferMethodInfo> m_enabledPlugins;
    QList<QVariantMap> m_pluginMetaData;
    Accounts::Manager *m_accountManager;
    QFileSystemWatcher *m_fileWatcher;
    QTimer *m_fileWatcherTimer;
    QTimer *m_delayedExitTimer;
    ClientActivityMonitor *m_activityMonitor;
    TransferEngine *q_ptr;
    QVariantList m_defaultActions;
    QVariant m_showTransfersAction;
    Q_DECLARE_PUBLIC(TransferEngine)
};

#endif

