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
 * Nome-Programma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nome-Programma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtQml>
#include <QGuiApplication>
#include <QQuickWindow>
#include <qpa/qplatformnativeinterface.h>

#include "element.h"
#include "client.h"
#include "grab.h"

static const int a = qmlRegisterType<Element>("Orbital", 1, 0, "Element");

int Element::s_id = 0;

Element::Element(Element *parent)
       : QQuickItem(parent)
       , m_type(Item)
       , m_parent(nullptr)
       , m_layout(nullptr)
{
    setParentElement(parent);
}

Element::~Element()
{
    if (m_parent) {
        m_parent->m_children.removeOne(this);
    }
    for (Element *elm: m_children) {
        elm->m_parent = nullptr;
    }
}

void Element::setId(int id)
{
    m_id = id;
    if (id > s_id) {
        s_id = id + 1;
    }
}

void Element::addProperty(const QString &name)
{
    m_properties << name;
}

void Element::publish()
{
    m_target = nullptr;
    Grab *grab = Client::createGrab();
    connect(grab, SIGNAL(focus(wl_surface *, int, int)), this, SLOT(focus(wl_surface *, int, int)));
    connect(grab, &Grab::motion, this, &Element::motion);
    connect(grab, &Grab::button, this, &Element::button);
}

void Element::focus(wl_surface *surface, int x, int y)
{
    if (m_target) {
        emit m_target->newElementExited(this);
    }

    QQuickWindow *window = Client::client()->findWindow(surface);
    QQuickItem *item = window->contentItem()->childAt(x, y);
    m_target = nullptr;
    while (!m_target && item) {
        m_target = qobject_cast<Element *>(item);
        item = item->parentItem();
    }

    if (m_target) {
        setParentItem(m_target);
        emit m_target->newElementEntered(this, x, y);
    }

    m_pos = QPointF(x, y);
}

void Element::motion(uint32_t time, int x, int y)
{
    if (m_target) {
        emit m_target->newElementMoved(this, x, y);
    }

    m_pos = QPointF(x, y);
}

void Element::button(uint32_t time, uint32_t button, uint32_t state)
{
    if (m_target) {
        setParentElement(m_target);
        emit m_target->newElementAdded(this, m_pos.x(), m_pos.y());
    } else {
        delete this;
    }

    static_cast<Grab *>(sender())->end();
}

void Element::setParentElement(Element *parent)
{
    if (parent == m_parent) {
        return;
    }

    if (m_parent) {
        m_parent->m_children.removeOne(this);
    }

    QQuickItem *content = (parent ? parent->property("content").value<QQuickItem *>() : nullptr);
    if (!content) {
        content = parent;
    }
    setParentItem(content);
    if (parent) {
        parent->m_children << this;
    }
    m_parent = parent;
}

void Element::sortChildren()
{
    if (m_sortProperty.isNull()) {
        return;
    }

    struct Sorter {
         QString property;

        bool operator()(Element *a, Element *b) {
            return QQmlProperty::read(a, property).toInt() < QQmlProperty::read(b, property).toInt();
        }
    };

    Sorter sorter = { m_sortProperty };
    qSort(m_children.begin(), m_children.end(), sorter);
}

Element *Element::create(QQmlEngine *engine, const QString &name, int id)
{
    QString path(QCoreApplication::applicationDirPath() + QLatin1String("/../src/client/"));
    QQmlComponent c(engine);
    c.loadUrl(path + name + ".qml");
    if (!c.isReady())
        qFatal(qPrintable(c.errorString()));

    QObject *obj = c.create();
    Element *elm = qobject_cast<Element *>(obj);
    if (!elm) {
        qDebug()<<name<<"not an element type.";
        return nullptr;
    }

    if (id < 0) {
        elm->m_id = s_id++;
    } else {
        elm->setId(id);
    }
    elm->m_typeName = name;

    return elm;
}
