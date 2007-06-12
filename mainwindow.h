/****************************************************************************
**
** Copyright (C) 2007 Urs Wolfer <uwolfer @ kde.org>
**
** This file is part of KDE.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; see the file COPYING. If not, write to
** the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA 02110-1301, USA.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class KToggleAction;
class KTabWidget;
class KUrlNavigator;

class FloatingToolBar;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotNewConnection();
    void slotQuit();
    void slotPreferences();
    void slotConfigureNotifications();
    void slotConfigureKeys();
    void slotConfigureToolbars();
    void slotShowMenubar();
    void resizeTabWidget(int w, int h);
    void showRemoteViewToolbar();
    void slotTakeScreenshot();
    void slotSwitchFullscreen();
    void slotLogout();
    void updateActionStatus();

private:
    void setupActions();

    QWidget *m_fullscreenWindow;
    QWidget *m_fullscreenWindowView;
    QByteArray m_mainWindowGeometry;

    KToggleAction *m_menubarAction;
    KTabWidget *m_tabWidget;
    KUrlNavigator *m_addressNavigator;

    FloatingToolBar *m_toolBar;
};

#endif