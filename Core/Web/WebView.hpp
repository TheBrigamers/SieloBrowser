/***********************************************************************************
** MIT License                                                                    **
**                                                                                **
** Copyright (c) 2017 Victor DENIS (victordenis01@gmail.com)                      **
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
#ifndef CORE_WEBVIEW_HPP
#define CORE_WEBVIEW_HPP

#include <QApplication>

#include <QEvent>
#include <QKeyEvent>

#include <QOpenGLWidget>
#include <QWidget>

#include <QWebEngineView>
#include <QWebEnginePage>

#include "Web/LoadRequest.hpp"
#include "Application.hpp"

namespace Sn {

class WebPage;
class WebHitTestResult;

class WebView: public QWebEngineView {
Q_OBJECT

public:
	static QList<int> zoomLevels();

	WebView(QWidget* parent = nullptr);
	~WebView();

	bool event(QEvent* event);
	bool eventFilter(QObject* watched, QEvent* event);

//	QIcon icon(bool allowNull = false) const;

	QString title() const;
	bool isTitleEmpty() const;

	WebPage* page() const;
	void setPage(WebPage* page);

	void load(const QUrl& url);
	void load(LoadRequest& request);
	bool isLoading();

	int loadingProgress() const;

	int zoomLevel() const;
	void setZoomLevel(int level);

	QPointF mapToViewport(const QPointF& pos) const;

	//TODO: History & Notification & ...

	QWidget* inputWidget() const;
	virtual QWidget* overlayWidget() = 0;

signals:
	void focusChanged(bool);
	void viewportResized(QSize);
	void privacyChanged(bool);
	void zoomLevelChanged(int);
	//TODO: Notifications signal

public slots:
	void zoomIn();
	void zoomOut();
	void zoomReset();

	void editUndo() { triggerPageAction(QWebEnginePage::Undo); }
	void editRedo() { triggerPageAction(QWebEnginePage::Redo); }
	void editCopy() { triggerPageAction(QWebEnginePage::Copy); }
	void editCut() { triggerPageAction(QWebEnginePage::Cut); }
	void editPast() { triggerPageAction(QWebEnginePage::Paste); }
	void editSelectAll() { triggerPageAction(QWebEnginePage::SelectAll); }
	void editDelet()
	{
		QKeyEvent event{QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier};
		QApplication::sendEvent(this, &event);
	}
	void reloadBypassCache() { triggerPageAction(QWebEnginePage::ReloadAndBypassCache); }

	void back();
	void forward();

	//TODO: Show sources
	void openUrlInNewTab(const QUrl& url, Application::NewTabType position) = 0;

	virtual void closeView() = 0;
	virtual void loadInNewTab(const LoadRequest& request, Application::NewTabType position) = 0;

	virtual bool isFullScreen() = 0;
	virtual void requestFullScreen(bool enable) = 0;

protected slots:
	void sLoadStarted();
	void sLoadProgress(int progress);
	void sLoadFinished(bool ok);
	void sUrlChanged(const QUrl& url);

	void openUrlInNewWindow();
	void copyLinkToClipboard();
	void savePageAs();
	void copyImageToClipboard();
	void dlLinkToDisk();
	void dlImageToDisk();
	void dlMediaToDisk();
	void openActionUrl();
	void showSiteInformation();
	void searchSelectedText();
	void searchSelectedTextInBgTab();
	void bookmarkLink();
	void openUrlInSelectedTab();
	void openUrlInBgTab();

protected:
	void resizeEvent(QResizeEvent* event);
	void contextMenuEvent(QContextMenuEvent* event);

	virtual void newWheelEvent(QWheelEvent* event);
	virtual void newMousePessEvent(QMouseEvent* event);
	virtual void newMouseReleaseEvent(QMouseEvent* event);
	virtual void newMouseMoveEvent(QMouseEvent* event);
	virtual void newKeyPressEvent(QKeyEvent* event);
	virtual void newKeyReleaseEvent(QKeyEvent* event);
	virtual void newContextMenuEvent(QContextMenuEvent* event);

	void loadRequest(const LoadRequest& request);
	void applyZoom();

	void createContextMenu(QMenu* menu, WebHitTestResult& hitTest);
	void createPageContextMenu(QMenu* menu);
	void createLinkContextMenu(QMenu* menu, const WebHitTestResult& hitTest);
	void createImageContextMenu(QMenu* menu, const WebHitTestResult& hitTest);
	void createSelectedTextContextMenu(QMenu* menu, const WebHitTestResult& hitTest);
	void createMediaContextMenu(QMenu* menu, const WebHitTestResult& hitTest);

private slots:
	//TODO: Manage forms (for password for example)

	void toggleMediaPause();
	void toggleMediaMute();

private:
	void initActions();

	int m_currentZoomLevel{};
	int m_progress{100};
	bool m_firstLoad{false};

	QUrl m_clickedUrl{};
	QPointF m_clickedPos{};

	WebPage* m_page{nullptr};
	QPointer<QOpenGLWidget> m_child{};
};
}

#endif //CORE_WEBVIEW_HPP