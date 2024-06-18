#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>

using namespace std;

// Function to calculate days between two dates by 30/360
int daysBetween(tuple<int, int, int> date1, tuple<int, int, int> date2) {
    int y1, m1, d1, y2, m2, d2;
    tie(y1, m1, d1) = date1;
    tie(y2, m2, d2) = date2;
    return (y2 - y1) * 360 + (m2 - m1) * 30 + (d2 - d1);
}

// Function to check if a year is a leap year
bool isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// Function to calculate days in month for actual/actual method
int daysInMonthFunc(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return -1; // Invalid month
    }
}

// Function to calculate days between two dates by actual/actual
int daysActualBetween(tuple<int, int, int> date1, tuple<int, int, int> date2){
	int y1, m1, d1, y2, m2, d2;
    tie(y1, m1, d1) = date1;
    tie(y2, m2, d2) = date2;
    
	int totaldays = 0;
	
	// y1 whole year 
	if (y1!=y2) totaldays += isLeapYear(y1) ? 366 : 365;
	// substract past months and days
	for (int i = 1; i <= m1 - 1; i++) totaldays -= daysInMonthFunc(y1, i);
	totaldays -= d1;
	// add middle years
	for (int j = y1 + 1; j <= y2 - 1; j++) totaldays += isLeapYear(j) ? 366 : 365;
	// add y2 months
    for (int k = 1; k <= m2 - 1; k++) totaldays += daysInMonthFunc(y2, k);
    // add y2 days
    totaldays += d2;
    
    return totaldays;
} 

// Function to calculate dirty price and clean price
void calculatePrices(tuple<int, int, int> maturityDate, tuple<int, int, int> settlementDate, double yield, double couponRate) {
    vector<tuple<int, int, int>> couponDates;

    // Calculate coupon dates
    int maturityYear, maturityMonth;
    tie(maturityYear, maturityMonth, ignore) = maturityDate;
    int settlementYear, settlementMonth;
    tie(settlementYear, settlementMonth, ignore) = settlementDate;
    
    int year = maturityYear;
    int month = maturityMonth;
    
    do {
        couponDates.push_back(make_tuple(year, month, 1));
        month -= 6;
        if (month <= 0) {
            month += 12;
            year--;
        }
    } while (year > settlementYear || (year == settlementYear && month > settlementMonth));
    // need to do it once again 
    couponDates.push_back(make_tuple(year, month, 1));
        month -= 6;
        if (month <= 0) {
            month += 12;
            year--;
        }
    
    // Check coupon date is correct 
    /*cout << "couponDates: ";
    for (const auto &date : couponDates) {
        cout << "(" << get<0>(date) << ", " << get<1>(date) << ", " << get<2>(date) << ") ";
    }
    cout << endl;
	*/
	
    // Calculate w for each coupon date
    double c = 100 * couponRate * 0.5;
    // last coupon date 
    tuple<int, int, int> lastTuple = couponDates[couponDates.size() - 1];
    // next coupon date 
	tuple<int, int, int> secondLastTuple = couponDates[couponDates.size() - 2];
    
    // Calculate w according to 30/360 method
    int daysToNextPayment = daysBetween(settlementDate, secondLastTuple);
    double w_30_360 = static_cast<double>(daysToNextPayment) / 180;

    // Calculate w according to actual/actual method
    // to next coupon date 
    int daysInCouponPeriod = daysActualBetween(settlementDate, secondLastTuple);
    // last coupon date to next coupon 
	int daysInMonth = daysActualBetween(lastTuple, secondLastTuple);
    double w_actual_actual = static_cast<double>(daysInCouponPeriod) / daysInMonth;

    // Calculate dirty price
    double dirtyPrice_30_360 = 0;
    double dirtyPrice_actual_actual = 0;
    int n = couponDates.size() - 1;
    yield = yield / 2;
    for (int i = 0; i < n; ++i) {
        dirtyPrice_30_360 += c * pow(1 + yield, -abs(w_30_360 + i));
        dirtyPrice_actual_actual += c * pow(1 + yield, -(w_actual_actual + i));
    }
    dirtyPrice_30_360 += 100 * pow(1 + yield, -(w_30_360 + n - 1));
    dirtyPrice_actual_actual += 100 * pow(1 + yield, -(w_30_360 + n - 1));

    // Calculate accrued interest
    double accruedInterest_30_360 = c * (1 - w_30_360);
    double accruedInterest_actual_actual = c * (1 - w_actual_actual);

    // Calculate clean price
    double cleanPrice_30_360 = dirtyPrice_30_360 - accruedInterest_30_360;
    double cleanPrice_actual_actual = dirtyPrice_actual_actual - accruedInterest_actual_actual;

    // Output results
    cout << "Dirty Price (30/360): " << dirtyPrice_30_360 << endl;
    cout << "Clean Price (30/360): " << cleanPrice_30_360 << endl;
    cout << "Dirty Price (Actual/Actual): " << dirtyPrice_actual_actual << endl;
    cout << "Clean Price (Actual/Actual): " << cleanPrice_actual_actual << endl;
    cout << "AccruedInterest_30_360: " << accruedInterest_30_360 << endl;
    cout << endl;

}

int main() {
    // Sample input
    tuple<int, int, int> maturityDate = make_tuple(2025, 11, 1);
    tuple<int, int, int> settlementDate = make_tuple(2024, 4, 18);
    double yield = 0.03; // 3%
    double couponRate = 0.03; // 10%

    calculatePrices(maturityDate, settlementDate, yield, couponRate);

    return 0;
}

