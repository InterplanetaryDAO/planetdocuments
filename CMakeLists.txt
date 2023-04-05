cmake_minimum_required(VERSION 3.9)

project(planetdocs)

find_package(eosio.cdt REQUIRED)

add_contract(${PROJECT_NAME} ${PROJECT_NAME} src/planetdocs.cpp)

