#include "../Date/Date.h"
#include "InvestAnnuity.h"
#include <cassert>
#include <math.h>
#include <cmath>

// Returns the payout amount
double InvestAnnuity(
	Date dt,			// Date (1/9/2021 - 31/12/2021) to buy product. 1st payment starts end-of-month
	int investPeriod,	// Investment period (years 1-5)
	int payment,			// monthly payment
	int payoutPeriod,	// Payout period (years 1-15)
	double interest		// Interest rate (%) on accrual amount
	) {

	assert(20210901 <= dt && dt <= 20211231);
	assert(1 <= investPeriod && investPeriod <= 10);
	assert(1 <= payoutPeriod && payoutPeriod <= 15);

	// TODO
	double balance = 0;
	Date curr = dt, next = monthend(dt);
	// payment period
	for (int i = 1; i <= 12 * investPeriod; ++i) {
		balance = balance * (1.0 + interest / 100.0 * dateDiff(curr, next) / 365) + payment; // balance earns interests and also contain new payment
		curr = next;
		next = nextMonthend(curr);
	}

	// use bisection to calculate payout such that balance equals to 0
	// by trying average of two payouts and replacing one of boundary payouts such that one makes balance < 0 and other makes balance > 0
	// bisection method intialization
	Date tcurr = curr, tnext = next;
	double lpayout = balance, rpayout = 0, tpayout = 0, tbalance = 0;
	// lpayout makes balance < 0, rpayout makes balanace > 0, tpayout as middle of these two
	do {
		// initialize iterators in each search
		tcurr = curr;
		tnext = next;
		tbalance = balance;
		tpayout = (lpayout + rpayout) / 2; // update tpayout in each search
		// payout period
		for (int i = 1; i <= 12 * payoutPeriod; ++i) {
			tbalance = tbalance * (1.0 + interest / 100.0 * dateDiff(tcurr, tnext) / 365) - tpayout; // balance earns interest and also minus payout
			tcurr = tnext;
			tnext = nextMonthend(tcurr);
		}
		if (tbalance > 0) // replace rpayout which makes balance > 0 by tpayout
			rpayout = tpayout;
		else  // replace lpayout which makes balance < 0 by tpayout
			lpayout = tpayout;
	} while (abs(tbalance) > 0.0000000001 && abs(rpayout - lpayout) > 0.0000001);
	// termination conditions: tbalance approximately equals to 0 or rpayout and lpayout are approximately close to each other

	return tpayout;
}