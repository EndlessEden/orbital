/*
 * Copyright 2013 Giulio Camuffo <giuliocamuffo@gmail.com>
 *
 * This file is part of Orbital
 *
 * Orbital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Orbital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Orbital.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UISCREEN_H
#define UISCREEN_H

#include <QObject>
#include <QQmlListProperty>
#include <QStringList>
#include <QRect>

class QXmlStreamReader;
class QXmlStreamWriter;
class QQmlEngine;
class QQuickItem;
class QScreen;

class Element;
class Client;
class ShellUI;
class Style;

class UiScreen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRect availableRect READ availableRect NOTIFY availableRectChanged)
public:
    UiScreen(ShellUI *ui, Client *client, int id, QScreen *screen);
    ~UiScreen();

    QScreen *screen() const { return m_screen; }
    int id() const { return m_id; }

    void loadConfig(QXmlStreamReader &xml);
    void saveConfig(QXmlStreamWriter &xml);
    void reloadConfig(QXmlStreamReader &xml);

    void addElement(Element *elm);
    void removeElement(Element *elm);

    QRect availableRect() const { return m_rect; }
    void setAvailableRect(const QRect &r);

signals:
    void availableRectChanged();

private:
    Element *loadElement(Element *parent, QXmlStreamReader &xml, QHash<int, Element *> *elements);
    void saveElement(Element *elm, QXmlStreamWriter &xml);
    void saveProperties(QObject *obj, const QStringList &properties, QXmlStreamWriter &xml);
    void saveChildren(const QList<Element *> &children, QXmlStreamWriter &xml);
    void elementDestroyed(QObject *obj);

    Client *m_client;
    ShellUI *m_ui;
    int m_id;
    QByteArray m_config;
    QScreen *m_screen;
    QRect m_rect;

    QHash<int, Element *> m_elements;
    QList<Element *> m_children;
};

#endif
