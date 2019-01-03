/**
 * This contract defines the TIP-5 Single Token Interface.
 * 
 * For a full developer walkthrough, see the README.md here: 
 * 
 * @author Craig Branscom
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("token.registry")]] registry : public contract {

public:

    registry(name self, name code, datastream<const char*> ds);

    ~registry();

    //NOTE: Developers edit here
    string const TOKEN_NAME = "Viita";
    symbol const TOKEN_SYM = symbol("VIITA", 4);
    asset const INITIAL_MAX_SUPPLY = asset(int64_t(100000000000000), TOKEN_SYM); //10 billion
    asset const INITIAL_SUPPLY = asset(int64_t(0), TOKEN_SYM);

    // ABI Actions
    [[eosio::action]] void mint(name recipient, asset tokens);

    [[eosio::action]] void transfer(name sender, name recipient, asset tokens);

    [[eosio::action]] void allot(name sender, name recipient, asset tokens);

    [[eosio::action]] void unallot(name sender, name recipient, asset tokens);

    [[eosio::action]] void claimallot(name sender, name recipient, asset tokens);

    [[eosio::action]] void createwallet(name recipient);

    [[eosio::action]] void deletewallet(name wallet_owner);
    
    /**
     * TokenConfig stores global token settings.
     * 
     * @scope singleton
     */
    struct [[eosio::table]] tokenconfig {
        name publisher;
        string token_name;
        asset max_supply;
        asset supply;

        uint64_t primary_key() const { return publisher.value; }
        EOSLIB_SERIALIZE(tokenconfig, (publisher)(token_name)(max_supply)(supply))
    };
    
    /**
     * Balances table stores native token balances.
     * 
     * @scope config.publisher.value (global)
     */
    struct [[eosio::table]] balance {
        name owner;
        asset tokens;

        uint64_t primary_key() const { return owner.value; }
        EOSLIB_SERIALIZE(balance, (owner)(tokens))
    };

    /**
     * Allotments table facilitates lazy transfers.
     * 
     * @scope sender.value (discoverable by sender)
     */
    struct [[eosio::table]] allotment {
        name recipient;
        name sender;
        asset tokens;

        uint64_t primary_key() const { return recipient.value; }
        //uint64_t by_sender() const { return sender.value; }
        EOSLIB_SERIALIZE(allotment, (recipient)(sender)(tokens))
    };

    typedef multi_index<name("balances"), balance> balances_table;

    typedef multi_index<name("allotments"), allotment> allotments_table;
        //indexed_by<name("sender"), const_mem_fun<allotment, uint64_t, &allotment::by_sender>>> allotments_table;

    typedef eosio::singleton<name("tokenconfig"), tokenconfig> config_singleton;
    config_singleton _config;
    tokenconfig config;

    //NOTE: Helper Functions
    void add_balance(name recipient, asset tokens);

    void sub_balance(name sender, asset tokens);

    void add_allot(name sender, name recipient, asset tokens);

    void sub_allot(name sender, name recipient, asset tokens);
};