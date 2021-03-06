/***********************************************************************************
** MIT License                                                                    **
**                                                                                **
** Copyright (c) 2018 Victor DENIS (victordenis01@gmail.com)                      **
**                                                                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
***********************************************************************************/

#pragma once
#ifndef SIELOBROWSER_HISTORYMANAGER_HPP
#define SIELOBROWSER_HISTORYMANAGER_HPP

#include <QList>
#include <QString>
#include <QUrl>

#include <QTimer>

#include "History/HistoryItem.hpp"

namespace Sn {
class HistoryModel;
class HistoryFilterModel;
class HistoryTreeModel;
class AutoSaver;

class HistoryManager: public QObject {
Q_OBJECT
	Q_PROPERTY(int historyLimit
				   READ
					   historyLimit
				   WRITE
				   setHistoryLimit)

public:
	struct HistoryEntryMatch {
		HistoryItem item{};
		QString match{};
	};

	explicit HistoryManager(QObject* parent = nullptr);
	~HistoryManager();

	bool historyContains(const QString& url) const;

	void addHistoryEntry(const QString& url);
	void removeHistoryEntry(const QString& url);

	void updateHistoryItem(const QUrl& url, const QString& title);

	int historyLimit() const { return m_historyLimit; }
	void setHistoryLimit(int limit);

	QList<HistoryItem>& history() { return m_history; }
	void setHistory(const QList<HistoryItem>& history, bool loadedAndSorted = false);

	QVector<HistoryEntryMatch> findEntries(const QString& prefix) const;

	HistoryModel* historyModel() const { return m_historyModel; }
	HistoryFilterModel* historyFilterModel() const { return m_historyFilterModel; }
	HistoryTreeModel* historyTreeModel() const { return m_historyTreeModel; }

	QString matchUrl(const QUrl& url, const QString& prefix) const;

signals:
	void historyReset();
	void entryAdded(const HistoryItem& item);
	void entryRemoved(const HistoryItem& item);
	void entryUpdate(int offset);

public slots:
	void clear();
	void loadSettings();

	void showDialog();

protected:
	void addHistoryItem(const HistoryItem& item);
	void removeHistoryItem(const HistoryItem& item);

private slots:
	void save();
	void checkForExpired(bool removeExpiredEntriesDirectly = false);

private:
	void load();

	AutoSaver* m_saveTimer{nullptr};

	int m_historyLimit{30};

	QTimer m_expiredTimer{};
	QList<HistoryItem> m_history;
	QString m_lastSavedUrl{};

	HistoryModel* m_historyModel{nullptr};
	HistoryFilterModel* m_historyFilterModel{nullptr};
	HistoryTreeModel* m_historyTreeModel{nullptr};

};
}

#endif //SIELOBROWSER_HISTORYMANAGER_HPP
