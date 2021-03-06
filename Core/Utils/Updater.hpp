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

#ifndef SIELOBROWSER_UPDATER_HPP
#define SIELOBROWSER_UPDATER_HPP

#include <QObject>

#include <QNetworkReply>

#include <QUrl>

namespace Sn {
class BrowserWindow;

class Updater: public QObject {
Q_OBJECT

public:
	Updater(BrowserWindow* window, QObject* parent = nullptr);

private slots:
	void downloadUpdateInfoCompleted();
	void downloadCompleted();

	void start();

private:
	void startDownloadingUpdateInfo(const QUrl& url);
	void startDownloadNewVersion(const QUrl& url);

	QNetworkReply* m_updaterReply{nullptr};
	QNetworkReply* m_versionReply{nullptr};

	BrowserWindow* m_window{nullptr};

	bool m_fullUpdate{false};
	bool m_themeUpdate{false};
};

}

#endif // SIELOBROWSER_UPDATER_HPP
