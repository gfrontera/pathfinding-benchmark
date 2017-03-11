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

#ifndef UES_MISC_CACHE_H
#define UES_MISC_CACHE_H

#include <cassert>
#include <vector>
#include <functional>

namespace ues
{
namespace misc
{

template < class Key,
         class T,
         class Hash = std::hash<Key>,
         class KeyEqual = std::equal_to<Key> >
class cache
{
public:
    typedef std::pair< Key, T > hash_pair;
    typedef std::vector< hash_pair > pair_vector;
    typedef typename pair_vector::iterator iterator;

    /** \name Constructor methods */
    /** \{ */

    cache();
    cache ( unsigned int max_size );

    cache ( const cache & other ) = delete;
    cache ( cache && other ) = default;

    ~cache() noexcept = default;

    /** \} */

    /** \name Operators */
    /** \{ */

    cache & operator= ( const cache & ) = delete;
    cache & operator= ( cache && ) = default;

    /** \} */

    /** Find the element in the cache by a \c Key. An iterator to the element will be
     * returned if found. */
    iterator find ( const Key & );

    /** Inserts an element in the cache. */
    void insert ( Key, T );
    /** Updates the value of an element in the cache. */
    void update ( const Key &, T );

    iterator end();

private:
    typedef std::vector<bool> used_cell_vector;

    unsigned int max_size;
    used_cell_vector used_cells;
    pair_vector cells;

};


// Template functions.

template< class Key, class T, class Hash, class KeyEqual >
cache<Key, T, Hash, KeyEqual>::cache()
    : cache ( 0 )
{
}


template< class Key, class T, class Hash, class KeyEqual >
cache<Key, T, Hash, KeyEqual>::cache ( unsigned int max_size )
    : max_size ( max_size ),
      used_cells ( max_size ),
      cells ( max_size )
{
}


template< class Key, class T, class Hash, class KeyEqual >
typename cache<Key, T, Hash, KeyEqual>::iterator cache<Key, T, Hash, KeyEqual>::find ( const Key & key )
{
    if ( max_size > 0 )
    {
        typename pair_vector::size_type pos = Hash() ( key ) % max_size;
        if ( used_cells[pos] && KeyEqual() ( key, cells[pos].first ) )
        {
            return cells.begin() + pos;
        }
    }
    return end();
}


template< class Key, class T, class Hash, class KeyEqual >
void cache<Key, T, Hash, KeyEqual>::insert ( Key key, T value )
{
    assert ( find ( key ) == end() );

    if ( max_size > 0 )
    {
        typename pair_vector::size_type pos = Hash() ( key ) % max_size;
        used_cells[pos] = true;
        cells[pos] = std::make_pair ( std::move ( key ), std::move ( value ) );
    }
}


template< class Key, class T, class Hash, class KeyEqual >
void cache<Key, T, Hash, KeyEqual>::update ( const Key & key, T value )
{
    assert ( find ( key ) != end() );

    iterator it = find ( key );
    it->second = std::move ( value );
}


template< class Key, class T, class Hash, class KeyEqual >
typename cache<Key, T, Hash, KeyEqual>::iterator cache<Key, T, Hash, KeyEqual>::end()
{
    return cells.end();
}


}
}

#endif // UES_MISC_CACHE_H
