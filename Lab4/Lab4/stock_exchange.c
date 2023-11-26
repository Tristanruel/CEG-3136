#include <stdlib.h>
#include "stock_exchange.h"
#include <limits.h>

static stock_exchange_t singleton_exchange;

void stock_exchange_init(uint32_t seed){
  int i;
  stock_exchange_t * exchange = &singleton_exchange;
  stock_t* stock = exchange->stocks;
  investor_t *investor = exchange->investors;
  char name_buffer[64];

  memset (exchange, 0, sizeof(stock_exchange_t));
  srand(seed);

  // initialize stocks
  for (i=0; i<NUM_STOCKS; i++) {
    uint16_t price = (rand() * 100. / RAND_MAX); 
    sprintf(name_buffer, "stock%04d", i);
    stock_initial_public_offering (stock, price+10, name_buffer);
    stock_display(stock);
    stock++;
  }
  // initialize investors
  for (i=0; i<NUM_INVESTORS; i++) {
    uint32_t cache = (rand() * 1000. / RAND_MAX); 
    sprintf(name_buffer, "investor%04d", i);
    investor_init (investor, name_buffer, 10000);
    investor_display(investor);
    investor++;
  }
}

stock_t* stock_exchange_get_random_stock(){
  int indx = rand() * 1. * NUM_STOCKS / RAND_MAX;
  if (indx == NUM_STOCKS) indx = NUM_STOCKS - 1;
  return &singleton_exchange.stocks[indx];
}

void stock_exchange_update(uint32_t time){
  uint32_t indx = time & (NUM_INVESTORS-1);
  investor_t *investor = &singleton_exchange.investors[indx];
  investor_add_transaction(investor);
}

void stock_exchange_display_stats(void){
  int a, b;
  uint32_t net_worth;
  int32_t gain_loss;
	
	int best_investor_id = -1, worst_investor_id = -1;
  int32_t best_gain = INT32_MIN, worst_gain = INT32_MAX;

  printf("Number of BUY transactions: %d \n", get_num_buy());
  printf("Number of SELL transactions: %d \n", get_num_sell());

  for (a = 0; a <= NUM_INVESTORS; ++a) {
    investor_t cur = singleton_exchange.investors[a];
    net_worth = cur.cache;

    for (b = 0; b < MAX_HOLDINGS; ++b) {
      if (cur.holdings[b].stock != NULL) {
        net_worth += cur.holdings[b].quantity * cur.holdings[b].stock->price;
      }
    }

    gain_loss = ((int64_t) net_worth - (int64_t) cur.initial_cache) * 100 / cur.initial_cache;
    printf("Investor %d: net_worth = %u, initial_cash = %u, gain/loss = %d%%\n", a, net_worth, cur.initial_cache, gain_loss);

    if (gain_loss > best_gain) {
      best_gain = gain_loss;
      best_investor_id = a;
    }
    if (gain_loss < worst_gain) {
      worst_gain = gain_loss;
      worst_investor_id = a;
    }
	if (a == 62){
  printf("Best Investor: Investor %d with a gain/loss of %d%%\n", best_investor_id, best_gain);
  printf("Worst Investor: Investor %d with a gain/loss of %d%%\n", worst_investor_id, worst_gain);
  }
	}
}

