/*
 * Copyright 2015-2017 Guillermo Frontera <guillermo.frontera@upm.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef UES_GUI_LOG_MODEL_H
#define UES_GUI_LOG_MODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include <QMutex>
#include <QFile>

#include <log/event.h>

namespace ues
{
namespace gui
{

class log_model : public QAbstractListModel
{
    Q_OBJECT

public:
    log_model ( const QString & filename, QObject * parent = nullptr );

    ~log_model() noexcept;

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const override;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const override;

    QJsonObject get_log ( const QModelIndex & index );
    QString get_log_message ( const QModelIndex & index );
    void add_event ( ues::log::event e );

    void set_minimum_level ( ues::log::level lvl ) noexcept;
    void set_component ( QString component_name );

    void clear() noexcept;

signals:
    void new_component ( QString );
    void cleared_components();

private:
    struct event_summary
    {
        qint64 position, size;
        ues::log::level lvl;
        unsigned int component;
        QString summary;
    };

    typedef std::vector< event_summary > event_vector;
    typedef std::map< std::string, unsigned int > component_numbers;

    event_vector log_data;
    component_numbers components;

    QMutex mutex, file_mutex;
    QFile log_file;

    ues::log::level minimum_level;
    unsigned int current_component;
};

}
}

#endif // UES_GUI_LOG_MODEL_H
