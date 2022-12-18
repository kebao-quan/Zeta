#include <iostream>
#include <math.h>
#include <string>
#include <cmath>
#include <vector>
#include <thread>
#include <functional>
#include <iomanip>
#include <chrono>
#include <gmpxx.h>
#include <gmp.h>

#define _USE_MATH_DEFINES



mpf_class pi(3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233785303861);

std::vector<mpz_class> primelist(uint64_t max_number) {
    using namespace std;
    // Set the maximum number up to which we want to generate prime numbers
    // Create a vector to store the prime numbers
    vector<mpz_class> primes;
    // Set all numbers from 2 to max_number as prime initially
    vector<bool> is_prime(max_number + 1, true);
    // Loop through all numbers from 2 to sqrt(max_number)
    for (uint64_t i = 2; i <= sqrt(max_number); i++) {
        // If the current number is marked as prime
        if (is_prime[i]) {
            // Loop through all multiples of the current number and mark them as not prime
            for (uint64_t j = i * i; j <= max_number; j += i) {
                is_prime[j] = false;
            }
        }
    }
    // Loop through all numbers from 2 to max_number
    for (uint64_t i = 2; i <= max_number; i++) {
        // If the current number is marked as prime, add it to the list of primes
        if (is_prime[i]) {
            mpz_class i_;
            i_.set_str(std::to_string(i), 10);
            primes.push_back(i_);
        }
    }
    return primes;
}
//
//
std::vector<mpz_class> primeList;
//
//
//
mpz_class product(std::vector<size_t> primeIndices)
{
    mpz_class p = 1;
    for (size_t i : primeIndices)
    { 
        if (i > (primeList.size() - 1))
        {
            std::cout << "i: " << i << std::endl;
            break;
        }
        p *= primeList.at(i);
    }
    return p;
}


mpz_class NtermRecursive(mpz_class l, std::vector<size_t> primeIndices, int power, int term)
{
    uint64_t i = 0;
    std::vector<size_t> primeIndices_copy = primeIndices;

    while ((i < primeIndices_copy.size()) && (primeIndices_copy[i] != -1))
    {
        i += 1;
    }


    if (i == primeIndices_copy.size())
    {
        mpz_class result;
        mpz_class div = l / product(primeIndices_copy);
        mpz_pow_ui(result.get_mpz_t(), div.get_mpz_t(), power);
        return result;
    }

    mpz_class sum = 0;

    while (1)
    {
        primeIndices_copy[i] += 1;

        if ((i > 0) && (primeIndices_copy[i] >= primeIndices_copy[i - 1]))
            break;


        std::vector<size_t> primeSubIndices = std::vector<size_t>(primeIndices_copy.begin(), primeIndices_copy.begin() + i + 1);

        if (product(primeSubIndices) > l)
            break;

        sum += NtermRecursive(l, primeIndices_copy, power, term);
    }
    return sum;
}


mpz_class Nterm(mpz_class l, int power, int term)
{
    std::vector<size_t> primeIndices(term, -1);
    mpz_class ret = NtermRecursive(l, primeIndices, power, term);
    std::cout << term << " " << ret << std::endl;
    return ret;
}


mpz_class N(mpz_class l, int power)
{
    short maxterm = 15;
    if (l <= pow(10, 10))
        maxterm = 12;
    if (l <= pow(10, 9))
        maxterm = 11;
    if (l <= pow(10, 8))
        maxterm = 9;
    if (l <= pow(10, 7))
        maxterm = 9;

    short term = 1;
    short sign = -1;
    mpz_class sum;
    mpz_pow_ui(sum.get_mpz_t(), l.get_mpz_t(), power);
    while (term < maxterm)
    {
        mpz_class t = Nterm(l, power, term);
        if (t == 0)
            break;
        sum += t * sign;
        sign = sign * (-1);
        std::cout << term << std::endl;
        term++;
    }
    return sum;
}



mpf_class NPioverL(mpz_class l, int power)
{
    mpz_class result;
    mpz_pow_ui(result.get_mpz_t(), l.get_mpz_t(), power);

    mpf_class pi_to_power;
    mpf_pow_ui(pi_to_power.get_mpf_t(), pi.get_mpf_t(), power);
    return N(l, power) * pi_to_power / result;
}







void generateCombinationsIterative(int n) {
    // Calculate the maximum number of combinations
    size_t max = std::pow(primeList.size(), n);
    // Loop through all possible combinations
    for (int i = 0; i < max; i++) {
        // Convert the counter to base p
        std::vector<size_t> indices;
        int x = i;
        while (x > 0) {
            indices.push_back(x % primeList.size());
            x /= primeList.size();
        }
        // Pad the indices with zeros if necessary
        while (indices.size() < n) {
            indices.push_back(0);
        }
        // Print the combination
        for (size_t index : indices) {
            std::cout << primeList[index] << " ";
        }
    }
}




int main()
{
    using std::cout;
    using std::endl;


    int L = pow(10,2);
    //

    //std::chrono::time_point<std::chrono::steady_clock> primestart = std::chrono::steady_clock::now();
    primeList = primelist(L);;
    //std::cout << "length of primeList: " << primeList.size() << std::endl;
    //std::chrono::time_point<std::chrono::steady_clock> primeend = std::chrono::steady_clock::now();
    //std::chrono::duration<double> prime_elapsed_seconds = primeend - primestart;
    //std::cout << "getting the primelist: " << prime_elapsed_seconds.count() << " seconds" << std::endl;


    //std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    //mpf_class result = NPioverL(L, 2);
    //cout << "result: " << std::setprecision(15) << result << endl;

    //std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    //std::chrono::duration<double> elapsed_seconds = end - start;
    //std::cout << "Getting the resutl in: " << elapsed_seconds.count() << " seconds" << std::endl;

    generateCombinationsIterative(3);





      
    return 0;


}