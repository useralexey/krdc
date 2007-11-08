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

#include "hostpreferenceslist.h"

#ifdef BUILD_VNC
#include "vnchostpreferences.h"
#endif
#ifdef BUILD_RDP
#include "rdphostpreferences.h"
#endif

#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KMessageBox>
#include <KPushButton>
#include <KStandardDirs>

#include <QFile>
#include <QLayout>
#include <QListWidget>

HostPreferencesList::HostPreferencesList(QWidget *parent)
        : QWidget(parent)
{
    hostList = new QListWidget(this);
    connect(hostList, SIGNAL(itemSelectionChanged()), SLOT(selectionChanged()));
    connect(hostList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(configureHost()));

    configureButton = new KPushButton(this);
    configureButton->setEnabled(false);
    configureButton->setText(i18n("Configure..."));
    configureButton->setIcon(KIcon("configure"));
    connect(configureButton, SIGNAL(clicked()), SLOT(configureHost()));

    removeButton = new KPushButton(this);
    removeButton->setEnabled(false);
    removeButton->setText(i18n("Remove"));
    removeButton->setIcon(KIcon("list-remove"));
    connect(removeButton, SIGNAL(clicked()), SLOT(removeHost()));

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(configureButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(hostList);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    readConfig();
}

HostPreferencesList::~HostPreferencesList()
{
}

void HostPreferencesList::readConfig()
{
    m_filename = KStandardDirs::locateLocal("appdata", "hostpreferences.xml");

    QFile file(m_filename);

    if (!m_doc.setContent(&file)) {
        kWarning(5010) << "Error reading " << m_filename;
        return;
    }

    QDomElement root = m_doc.documentElement();
    for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
        if (n.toElement().hasAttribute("url"))
            hostList->addItem(new QListWidgetItem(n.toElement().attribute("url")));
    }
}

void HostPreferencesList::saveSettings()
{
}

void HostPreferencesList::configureHost()
{
    QList<QListWidgetItem *> selectedItems = hostList->selectedItems();

    foreach(QListWidgetItem *selectedItem, selectedItems) {
        QString url = selectedItem->text();

        kDebug(5010) << "Configure host: " << url;

#ifdef BUILD_VNC
        if (url.startsWith("vnc", Qt::CaseInsensitive))
            VncHostPreferences hostPreferences(url, true, this);
        else
#endif
#ifdef BUILD_RDP
        if (url.startsWith("rdp", Qt::CaseInsensitive))
            RdpHostPreferences hostPreferences(url, true, this);
        else
#endif
            KMessageBox::error(this,
                               i18n("The selected host cannot be handled."),
                               i18n("Unusable URL"));
    }
}

void HostPreferencesList::removeHost()
{
    QList<QListWidgetItem *> selectedItems = hostList->selectedItems();

    foreach(QListWidgetItem *selectedItem, selectedItems) {
        kDebug(5010) << "Remove host: " <<  selectedItem->text();

        QDomElement root = m_doc.documentElement();
        for (QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
            if (n.toElement().hasAttribute("url") && n.toElement().attribute("url") == selectedItem->text()) {
                kDebug(5010) << "Found and remove now: " << selectedItem->text();
                root.removeChild(n);
            }
        }
        delete(selectedItem);
    }

    QFile file(m_filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        kWarning(5010) << "Cannot write " << m_filename << ". " << file.errorString() << endl;
    }

    QTextStream out(&file);
    m_doc.save(out, 4);

    hostList->clearSelection();
}

void HostPreferencesList::selectionChanged()
{
    bool enabled = hostList->selectedItems().isEmpty() ? false : true;

    configureButton->setEnabled(enabled);
    removeButton->setEnabled(enabled);
}

#include "hostpreferenceslist.moc"