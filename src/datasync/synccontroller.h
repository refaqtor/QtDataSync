#ifndef QTDATASYNC_SYNCCONTROLLER_H
#define QTDATASYNC_SYNCCONTROLLER_H

#include "QtDataSync/qtdatasync_global.h"

#include <QtCore/qobject.h>

#include <functional>

namespace QtDataSync {

class SyncControllerPrivate;
//! A controller class to monitor and control a datasync instance
class Q_DATASYNC_EXPORT SyncController : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool syncEnabled READ isSyncEnabled WRITE setSyncEnabled NOTIFY syncEnabledChanged)
	//! The current synchronization state
	Q_PROPERTY(SyncState syncState READ syncState NOTIFY syncStateChanged)
	//! Holds authentication errors, if they did occur
	Q_PROPERTY(QString authenticationError READ authenticationError NOTIFY authenticationErrorChanged)

public:
	//! Defines all possible synchronization states
	enum SyncState {
		Loading,//!< Loading the remote state
		Disconnected,//!< Remote is not available
		Syncing,//!< Remote and local state loaded, comparing them and syncing changes
		Synced,//!< Finished synchronization successfully
		SyncedWithErrors//!< Finished synchronization, but with errors
	};
	Q_ENUM(SyncState)

	//! Constructs a controller for the default setup
	explicit SyncController(QObject *parent = nullptr);
	//! Constructs a controller for the given setup
	explicit SyncController(const QString &setupName, QObject *parent = nullptr);
	//! Destructor
	~SyncController();

	//! @readAcFn{SyncController::syncEnabled}
	bool isSyncEnabled() const;
	//! @readAcFn{SyncController::syncState}
	SyncState syncState() const;
	//! @readAcFn{SyncController::authenticationError}
	QString authenticationError() const;

public Q_SLOTS:
	//! Tells the datasync instance to start synchronizing
	void triggerSync();
	//! Tells the datasync instance to start synchronizing, with a result handler
	void triggerSyncWithResult(std::function<void(SyncState)> resultFn);
	//! Tells the datasync instance to start a resynchronization
	void triggerResync();
	//! Tells the datasync instance to start a resynchronization, with a result handler
	void triggerResyncWithResult(std::function<void(SyncState)> resultFn);

	//! @writeAcFn{SyncController::syncEnabled}
	void setSyncEnabled(bool syncEnabled);

Q_SIGNALS:
	//! @notifyAcFn{SyncController::syncEnabled}
	void syncEnabledChanged(bool syncEnabled);
	//! @notifyAcFn{SyncController::syncState}
	void syncStateChanged(SyncState syncState);
	//! Is emitted with the current count of remaining datasets to be synced
	void syncOperationsChanged(int remainingOperations);
	//! @notifyAcFn{SyncController::authenticationError}
	void authenticationErrorChanged(const QString &authenticationError);

private:
	QScopedPointer<SyncControllerPrivate> d;

	void setupTriggerResult(std::function<void(SyncState)> resultFn);
};

}

#endif // QTDATASYNC_SYNCCONTROLLER_H
