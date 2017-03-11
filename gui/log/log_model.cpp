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

#include "log_model.h"

#include <QJsonDocument>

#include <exc/exception.h>
#include <log/util.h>
#include <ser/serializer.h>

using namespace ues::gui;


log_model::log_model ( const QString & filename, QObject * parent )
    : QAbstractListModel ( parent ),
      log_file ( filename )
{
    if ( !log_file.open ( QIODevice::ReadWrite | QIODevice::Truncate ) )
    {
        throw ues::exc::exception ( "Cannot open log file with write permissions", UES_CONTEXT );
    }
}


log_model::~log_model() noexcept
{
    log_file.close();
}


int log_model::rowCount ( const QModelIndex & parent ) const
{
    if ( parent.isValid() )
    {
        return 0;
    }
    else
    {
        return log_data.size();
    }
}


QVariant log_model::data ( const QModelIndex & index, int role ) const
{
    if ( index.isValid() )
    {
        if ( role == Qt::DisplayRole )
        {
            const event_summary & es = log_data.at ( index.row() );
            return "[ " + QString::fromStdString ( ues::log::describe_level ( es.lvl ) ) + " ] " + es.summary;
        }
    }

    return QVariant();
}


void log_model::add_event ( ues::log::event e )
{
    event_summary es;
    es.lvl = e.get_level();
    es.summary = QString::fromStdString ( e.get_summary() );
    auto comp_it = components.find ( e.get_component() );
    if ( comp_it != components.end() )
    {
        es.component = comp_it->second;
    }
    else
    {
        es.component = components.size() + 1;
        components.insert ( std::make_pair ( e.get_component(), es.component ) );
        emit new_component ( QString::fromStdString ( e.get_component() ) );
    }

    QJsonDocument doc ( ues::ser::to_json ( e ) );
    QByteArray bytes = doc.toJson();

    try
    {
        file_mutex.lock();

        es.position = log_file.pos();
        es.size = log_file.write ( std::move ( bytes ) );
        log_file.flush();

        file_mutex.unlock();
    }
    catch ( std::exception & e )
    {
        file_mutex.unlock();
        throw;
    }

    mutex.lock();

    beginInsertRows ( QModelIndex(), log_data.size(), log_data.size() );
    log_data.push_back ( std::move ( es ) );
    endInsertRows();

    mutex.unlock();

}


void log_model::set_minimum_level ( ues::log::level lvl ) noexcept
{
    minimum_level = lvl;
}


void log_model::set_component ( QString component_name )
{
    if ( component_name == "All components" )
    {
        current_component = 0;
    }
    else
    {
        auto component_it = components.find ( component_name.toStdString() );
        if ( component_it != components.end() )
        {
            current_component = component_it->second + 1;
        }
        else
        {
            throw ues::exc::exception ( "Invalid component name \"" + component_name.toStdString() + "\"", UES_CONTEXT );
        }
    }
}


void log_model::clear() noexcept
{
    mutex.lock();

    beginRemoveRows ( QModelIndex(), 0, log_data.size() - 1 );
    log_data.clear();
    endRemoveRows();

    components.clear();
    current_component = 0;
    emit cleared_components();
    mutex.unlock();

    file_mutex.lock();
    log_file.reset();
    log_file.resize ( 0 );
    file_mutex.unlock();
}


QJsonObject log_model::get_log ( const QModelIndex & index )
{
    qint64 position, size;
    position = log_data.at ( index.row() ).position;
    size = log_data.at ( index.row() ).size;

    try
    {
        file_mutex.lock();

        qint64 prev_pos = log_file.pos();
        if ( log_file.seek ( position ) )
        {
            QByteArray bytes = log_file.read ( size );
            if ( !log_file.seek ( prev_pos ) )
            {
                throw ues::exc::exception ( "Unable to restore write position in log", UES_CONTEXT );
            }

            file_mutex.unlock();

            QJsonDocument doc ( QJsonDocument::fromJson ( std::move ( bytes ) ) );

            return doc.object();

        }
        else
        {
            throw ues::exc::exception ( "Unable to read from log", UES_CONTEXT );
        }
    }
    catch ( std::exception & e )
    {
        file_mutex.unlock();
        throw;
    }
}


QString log_model::get_log_message ( const QModelIndex & index )
{
    QJsonObject json = get_log ( index );
    return json["message"].toString();
}
