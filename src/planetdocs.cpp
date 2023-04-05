#include "../include/planetdocs.hpp"
#include "../include/config.h"
#include <cmath>

using namespace eosio;
using namespace std;

// constructor
planetdocs::planetdocs(name self, name code, datastream<const char *> ds) : contract(self, code, ds),
                                                                    documents(this->_self, this->_self.value), // code and scope both set to the contract's wallet
                                                                    constitution(this->_self, this->_self.value), // code and scope both set to the contract's wallet
                                                                    events(this->_self, this->_self.value) // code and scope both set to the contract's wallet

   
                                                                            {}

/** destructor
voting::voting()
{
}
*/



void planetdocs::updconstitut(eosio::name wallet, eosio::name planet, string content)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    eosio::multi_index<"constitution"_n, constitution_t> constitution_table(get_self(), planet.value);


    auto itr = constitution_table.find(planet.value);
    if (itr != constitution_table.end())
    {
        constitution_table.modify( itr, _self, [&]( auto& row ) {
            row.content = content;
            row.version = row.version +1;
            row.last_change = current_time_point();
            row.last_change_by = wallet;
        });

    } else
    {
        constitution_table.emplace(_self, [&](auto &row) {
            row.planet_name = planet;
            row.content = content;
            row.version = 1;
            row.last_change = current_time_point();
            row.last_change_by = wallet;
        });
    }

}



void planetdocs::adddoc(eosio::name wallet, eosio::name planet, string title, string content)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    eosio::multi_index<"documents"_n, documents_t> documents_table(get_self(), planet.value);

    documents_table.emplace(_self, [&](auto &row) {
        row.doc_id = documents_table.available_primary_key();
        row.title = title;
        row.content = content;
        row.version = 1;
        row.time_submitted = current_time_point();
        row.last_change = current_time_point();
        row.last_change_by = wallet;
    });


}


void planetdocs::deletedoc(eosio::name wallet, eosio::name planet, uint64_t doc_id)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    eosio::multi_index<"documents"_n, documents_t> documents_table(get_self(), planet.value);
    auto itr_document = documents_table.require_find(doc_id, "This document does not exist for the selected planet");

    itr_document = documents_table.erase(itr_document);

}




void planetdocs::updatedoc(eosio::name wallet, eosio::name planet, uint64_t doc_id, string title, string content)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    eosio::multi_index<"documents"_n, documents_t> documents_table(get_self(), planet.value);
    auto itr_document = documents_table.require_find(doc_id, "This document does not exist for the selected planet");

    documents_table.modify(itr_document, _self, [&](auto &row) {
        row.title = title;
        row.content = content;
        row.version = row.version + 1;
        row.time_submitted = current_time_point();
        row.last_change = current_time_point();
        row.last_change_by = wallet;
    }); 

}


void planetdocs::addevent(eosio::name wallet, eosio::name planet, string title, string content, eosio::time_point_sec event_start_utc, bool use_fixed_end, eosio::time_point_sec event_end_utc, bool use_end_condition, string alt_end_condition)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    events.emplace(_self, [&](auto &row) {
        row.event_id = events.available_primary_key();
        row.planet = planet;
        row.title = title;
        row.content = content;
        row.event_start_utc = event_start_utc;
        row.use_fixed_end = use_fixed_end;
        row.event_end_utc = event_end_utc;
        row.use_end_condition = use_end_condition;
        row.alt_end_condition = alt_end_condition;
        row.version = 1;
        row.time_submitted = current_time_point();
        row.last_change = current_time_point();
        row.last_change_by = wallet;
    });


}



void planetdocs::updateevent(eosio::name wallet, eosio::name planet, uint64_t event_id, string title, string content, eosio::time_point_sec event_start_utc, bool use_fixed_end, eosio::time_point_sec event_end_utc, bool use_end_condition, string alt_end_condition)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    auto itr_event = events.require_find(event_id, "An event with this ID does not exist");

    events.modify(itr_event, _self, [&](auto &row) {
        row.title = title;
        row.content = content;
        row.event_start_utc = event_start_utc;
        row.use_fixed_end = use_fixed_end;
        row.event_end_utc = event_end_utc;
        row.use_end_condition = use_end_condition;
        row.alt_end_condition = alt_end_condition;
        row.version = row.version + 1;
        row.last_change = current_time_point();
        row.last_change_by = wallet;
    });


}



void planetdocs::deleteevent(eosio::name wallet, uint64_t event_id)
{

    require_auth(wallet);

    //custodians_table awcustodians("dao.worlds"_n, planet.value);
    //auto itr_awcustodian = awcustodians.require_find(wallet.value, "You are not a custodian on this planet. Possible planets are: eyeke, nerix, kavian, naron, magor and veles.");
    
    auto itr_event = events.require_find(event_id, "An event with this ID does not exist");
    itr_event = events.erase(itr_event);

}