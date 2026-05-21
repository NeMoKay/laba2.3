#include "Fixtures.hpp"

TEST_F(Dict_Fixture, set_and_get){
    dict->Set(Pair<int, int>(1, 2), 10.5);
    EXPECT_EQ(dict->Get(Pair<int, int>(1, 2)), 10.5) << "Ожидаемое значение 10.5. По факту: " << dict->Get(Pair<int, int>(1, 2));
}

TEST_F(Dict_Fixture, set_update_key){
    dict->Set(Pair<int, int>(1, 1), 5.0);
    dict->Set(Pair<int, int>(1, 1), 15.0);
    EXPECT_EQ(dict->Get(Pair<int, int>(1, 1)), 15.0) << "Ожидаемое обновленное значение 15.0. По факту: " << dict->Get(Pair<int, int>(1, 1));
}

TEST_F(Dict_Fixture, get_nonexistent_key){
    EXPECT_EQ(dict->Get(Pair<int, int>(99, 99)), 0.0) << "Ожидаемое значение для пустого ключа 0.0. По факту: " << dict->Get(Pair<int, int>(99, 99));
}

TEST_F(Dict_Fixture, collision_handling){
    dict->Set(Pair<int, int>(1, 0), 1.0);
    dict->Set(Pair<int, int>(2, 31), 2.0);
    dict->Set(Pair<int, int>(5, 5), 5.5);
    
    EXPECT_EQ(dict->Get(Pair<int, int>(1, 0)), 1.0) << "Ожидаемое значение 1.0. По факту: " << dict->Get(Pair<int, int>(1, 0));
    EXPECT_EQ(dict->Get(Pair<int, int>(2, 31)), 2.0) << "Ожидаемое значение 2.0. По факту: " << dict->Get(Pair<int, int>(2, 31));
    EXPECT_EQ(dict->Get(Pair<int, int>(5, 5)), 5.5) << "Ожидаемое значение 5.5. По факту: " << dict->Get(Pair<int, int>(5, 5));
}

TEST_F(Dict_Fixture, get_items_count){
    dict->Set(Pair<int, int>(1, 1), 1.0);
    dict->Set(Pair<int, int>(2, 2), 2.0);
    dict->Set(Pair<int, int>(3, 3), 3.0);
    
    auto items = dict->Get_Items();
    EXPECT_EQ(items.GetLength(), 3) << "Ожидаемое количество элементов 3. По факту: " << items.GetLength();
}