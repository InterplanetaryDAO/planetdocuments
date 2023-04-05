#pragma once
#include <eosio/eosio.hpp>
#include <eosio/name.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/datastream.hpp>
#include <eosio/asset.hpp>
#include <cmath>

using namespace eosio;
using namespace std;



CONTRACT planetdocs : public eosio::contract
{
  public:
    // constructor
    planetdocs(name self, name code, datastream<const char *> ds);

    // destructor
    planetdocs();


//INTERACTIONS

    /**
     * Updates the constituation
     *
     * @Updates the constitution of a planet
     */
    ACTION updconstitut(eosio::name wallet, eosio::name planet, string content);


    /**
     * Updates or deletes a document
     *
     * @Updates or deletes a document from the documents table
     */
    ACTION updatedoc(eosio::name wallet, eosio::name planet, uint64_t doc_id, string title, string content);


    /**
     * adds a document
     *
     * @adds a document from the documents table
     */
    ACTION adddoc(eosio::name wallet, eosio::name planet, string title, string content);


    /**
     * deletes a document
     *
     * @deletes a document from the documents table
     */
    ACTION deletedoc(eosio::name wallet, eosio::name planet, uint64_t doc_id);



    /**
     * Updates or deletes an event
     *
     * @Updates or deletes an event from the events table
     */
    ACTION updateevent(eosio::name wallet, eosio::name planet, uint64_t event_id, string title, string content, eosio::time_point_sec event_start_utc, bool use_fixed_end, eosio::time_point_sec event_end_utc, bool use_end_condition, string alt_end_condition);


    /**
     * adds an event
     *
     * @adds an event from the events table
     */
    ACTION addevent(eosio::name wallet, eosio::name planet, string title, string content, eosio::time_point_sec event_start_utc, bool use_fixed_end, eosio::time_point_sec event_end_utc, bool use_end_condition, string alt_end_condition);


    /**
     * deletes an event
     *
     * @deletes an event from the events table
     */
    ACTION deleteevent(eosio::name wallet, uint64_t event_id);

//TABLES

    /**
     * Table to store information of available planets
     *
     * @param {name} planet_name - the name of the planets scope (eg. eyeke, nerix, kavian, ...)
     * @param {string} content - the content of the constitution
     * @param {int} version - the count of changes made to this document
     * @param {time_point_sec} last_change - the time at which this document was edited
     * @param {name} last_change_by - the wallet which submitted or edited the document
     */

    TABLE constitution_t
    {
        eosio::name planet_name;
        std::string content;
        uint64_t version;
        eosio::time_point_sec last_change;
        eosio::name last_change_by;
        uint64_t primary_key() const { return planet_name.value; }
    };


    /**
     * Table to store the planets documents
     *
     * @param {int} doc_id - An auto-incrementing ID for a document
     * @param {string} title - the title for the document (eg. "Proposal Submission")
     * @param {string} content - the content of the document
     * @param {int} version - the count of changes made to this document
     * @param {time_point_sec} time_submitted - the time at which this document was submitted
     * @param {time_point_sec} last_change - the time at which this document was edited
     * @param {name} last_change_by - the wallet which submitted or edited the document
     */
    
    TABLE documents_t
    {
        uint64_t doc_id;
        std::string title;
        std::string content;
        uint64_t version;
        eosio::time_point_sec time_submitted;
        eosio::time_point_sec last_change;
        eosio::name last_change_by;
        uint64_t primary_key() const { return doc_id; }
    };


    /**
     * Table to store planetary events
     *
     * @param {int} event_id - An auto-incrementing ID for an event
     * @param {string} title - the title for the document (eg. "Proposal Submission")
     * @param {string} content - the content of the document
     * @param {time_point_sec} event_start_utc - the time at which the event starts
     * @param {bool} use_fixed_end - if the event uses a fixed time when it will end
     * @param {time_point_sec} event_end_utc - the time at which the event ends if use_fixed_end is true
     * @param {bool} use_end_condition - if the event uses an alternative end condition
     * @param {string} alt_end_condition - the alternative end condition
     * @param {int} version the count of changes made to this document
     * @param {time_point_sec} time_submitted - the time at which this document was submitted
     * @param {time_point_sec} last_change - the time at which this document was edited
     * @param {name} last_change_by - the wallet which submitted or edited the document
     */
    
    TABLE events_t
    {
        uint64_t event_id;
        eosio::name planet;
        std::string title;
        std::string content;
        eosio::time_point_sec event_start_utc;
        bool use_fixed_end;
        eosio::time_point_sec event_end_utc;
        bool use_end_condition;
        std::string alt_end_condition;
        uint64_t version;
        eosio::time_point_sec time_submitted;
        eosio::time_point_sec last_change;
        eosio::name last_change_by;
        uint64_t primary_key() const { return event_id; }
        uint64_t secondary_key1() const { return event_start_utc.sec_since_epoch(); }
    };


  //DEFNINITIONS

    eosio::multi_index<"constitution"_n, constitution_t> constitution;
    eosio::multi_index<"documents"_n, documents_t> documents;

    eosio::multi_index<"events"_n, events_t,
            indexed_by<"eventstart"_n, const_mem_fun<events_t,uint64_t,&events_t::secondary_key1>>>
    events;

    //structure of the custodians1 table used in the dao.worlds contract
    struct [[eosio::table("custodians1"), eosio::contract("documents_t")]] custodian {
        eosio::name           cust_name;
        eosio::asset          requestedpay;
        uint64_t              total_vote_power;
        uint64_t              rank;
        uint32_t              number_voters;
        eosio::time_point_sec avg_vote_time_stamp;

        uint64_t primary_key() const {
            return cust_name.value;
        }

        uint64_t by_votes_rank() const {
            return 0;
        }

        uint64_t by_decayed_votes() const {
            return 0;
        }

        uint64_t by_requested_pay() const {
            return 0;
        }
    };


    typedef eosio::multi_index<"custodians1"_n, custodian,
        eosio::indexed_by<"byvotesrank"_n, eosio::const_mem_fun<custodian, uint64_t, &custodian::by_votes_rank>>,
        eosio::indexed_by<"bydecayed"_n, eosio::const_mem_fun<custodian, uint64_t, &custodian::by_decayed_votes>>,
        eosio::indexed_by<"byreqpay"_n, eosio::const_mem_fun<custodian, uint64_t, &custodian::by_requested_pay>>> custodians_table;
  
};