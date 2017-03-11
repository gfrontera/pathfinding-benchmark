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

#include "serializer.h"

#include <log/event.h>
#include <log/util.h>


QJsonObject ues::ser::to_json ( const ues::log::event & e )
{
    QJsonObject result;
    result["timestamp"] = QString::fromStdString ( boost::posix_time::to_iso_string ( e.get_timestamp() ) );
    result["level"] = QString::fromStdString ( ues::log::describe_level ( e.get_level() ) );
    result["component"] = QString::fromStdString ( e.get_component() );
    result["summary"] = QString::fromStdString ( e.get_summary() );
    result["message"] = QString::fromStdString ( e.get_message() );
    return result;
}

ues::log::event ues::ser::from_json ( const QJsonObject & e )
{
    ues::log::event result ( ues::log::reverse_describe_level ( e["level"].toString().toStdString() ),
                             e["component"].toString().toStdString(),
                             e["summary"].toString().toStdString(),
                             boost::posix_time::from_iso_string ( e["timestamp"].toString().toStdString() ) );
    result.message() << e["message"].toString().toStdString();
    return result;
}
