#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT wxlaunches : public contract {
  public:
    using contract::contract;

    ACTION addobs(uint64_t pressure, float temperature, float humidity, name miner);

  private:

    TABLE observations {
      uint64_t pressure_hpa;
      float temperature_c;
      float relHumidity;

      auto  primary_key() const { return pressure_hpa; }
    };
    typedef multi_index<name("observations"), observations> observations_table;

};
