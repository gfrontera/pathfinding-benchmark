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

#ifndef UES_SER_SERIALIZER_H
#define UES_SER_SERIALIZER_H

#include <QJsonObject>

namespace ues
{

// Declarations of serialized types

namespace log
{
class event;
}

namespace ser
{

QJsonObject to_json ( const ues::log::event & );
ues::log::event from_json ( const QJsonObject & );

}
}

#endif // UES_SER_SERIALIZER_H
