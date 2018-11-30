/**
 * 
 * 
 * @author Craig Branscom
 * @copyright defined in telos/LICENSE.txt
 */

#include <trail.voting.hpp>
#include <trail.tokens.hpp>

#include <eosiolib/eosio.hpp>
#include <eosiolib/permission.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/singleton.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("telos.tfvt")]] telfound : public contract {

public:

    telfound(name self, name code, datastream<const char*> ds);

    ~telfound();


    struct [[eosio::table]] board_nominee {
        name nominee;
        
        uint64_t primary_key() const { return nominee.value; }
        EOSLIB_SERIALZE(board_nominee, (nominee))
    };

    struct [[eosio::table]] board_member {
        name member;

        uint64_t primary_key() const { return member.value; }
        EOSLIB_SERIALZE(board_member, (member))
    };

    struct [[eosio::table]] config {
        name publisher;
        uint8_t max_board_seats = 12; //NOTE: adjustable by board members
        uint8_t open_seats = 0;

        uint64_t primary_key() const { return publisher.value; }
        EOSLIB_SERIALZE(config, (publisher)(max_board_seats)(open_seats))
    };

    
    typedef multi_index<name("nominees"), board_nominee> nominees_table;

    typedef multi_index<name("boardmembers"), board_member> members_table;

    typedef singleton<name("configs"), config> config_table;


    [[eosio::action]]
    void setconfig(name publisher, uint8_t new_max_seats, uint8_t new_open_seats);

    [[eosio::action]]
    void nominate(name nominee, name nominator);

    [[eosio::action]]
    void makeissue(name holder, uint32_t begin_time, uint32_t end_time, string info_url); //NOTE: ballot type is 0, symbol is TFVT

    [[eosio::action]]
    void closeissue(name holder, uint64_t ballot_id, uint8_t status); //TODO: sends inline action from self

    [[eosio::action]]
    void holdelection(name holder, uint32_t begin_time, uint32_t end_time, string info_url);

    [[eosio::action]]
    void closeelec(name holder, uint64_t ballot_id, uint8_t status);



    #pragma region Helper_Functions

    void add_to_tfboard(name nominee);

    void rmv_from_tfboard(name member);

    void addseats(name member, uint8_t num_seats);

    bool is_board_member(name user);

    bool is_nominee(name user);

    bool is_tfvt_holder(name user);

    bool is_tfboard_holder(name user);

    #pragma endregion Helper_Functions

};
