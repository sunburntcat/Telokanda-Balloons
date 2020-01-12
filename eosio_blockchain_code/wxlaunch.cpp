#include <wxlaunches.hpp>
#include <eosio/asset.hpp>

using namespace std;
using namespace eosio;

ACTION wxlaunches::addobs(uint64_t pressure, float temperature, float humidity, name miner) {
  
  // Only self can run this Action
  require_auth(get_self());

  // Init the _message table
  observations_table _observations(get_self(), get_self().value);

  // Gets lowest observation with lowest pressure that's greater than our current obs
  auto obs_itr = _observations.lower_bound(pressure);

  // Our observation is the first below 300hPa
  if (pressure < 300 && obs_itr->pressure_hpa > 300) {
    // Set reward asset. A value of 10000 is equivalent to 1 EOS
    eosio::asset reward = eosio::asset( 10000, symbol(symbol_code("EOS"),4));

    // Use an inline action to send reward to miner
    action(
    permission_level{ get_self(), "active"_n },
    "eosio.token"_n, "transfer"_n,
    std::make_tuple( get_self(), miner, reward, string("Thank you for launching!"))
  ).send();
  }

  // Add the row to the observation set
  _observations.emplace(get_self(), [&](auto& obs) {
      obs.pressure_hpa = pressure;
      obs.temperature_c = temperature;
      obs.relHumidity = humidity;
    });

}

// Dispatch the actions to the blockchain
EOSIO_DISPATCH(wxlaunches, (addobs))
