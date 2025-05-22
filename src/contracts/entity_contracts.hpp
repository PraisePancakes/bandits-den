#pragma once
#include "../../vendor/SnakeECS/snakeecs/utils/type_list.hpp"
#include <iostream>

namespace bden::gamelayer::contracts
{
    /*

        using player_contract = entity_contract<T, U, V>;
        entity_type<player_contract> player{T(), U(), V()};
    */
    template <typename... Components>
    struct entity_contract
    {
        using component_list = snek::utils::type_list<Components...>;
        static_assert((std::is_constructible_v<Components> && ...), "components must be constructible");
    };

};