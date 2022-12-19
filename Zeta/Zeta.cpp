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
#include <primesieve.hpp>
#define _USE_MATH_DEFINES



mpf_class pi(3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233785303861);



std::vector<mpz_class> generatePrimes(size_t max)
{
    std::vector<mpz_class> primeList;

    primesieve::iterator it;
    uint64_t prime = it.next_prime();

    for (; prime < max; prime = it.next_prime())
    {
        mpz_class tmp(std::to_string(prime));
        primeList.push_back(tmp);
    }


    return primeList;
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

mpz_class leastPrimeProduct(int n)
{
    mpz_class p = 1;
    for (int i = 0; i < n; i++)
    {
        p *= primeList.at(i);
    }
    return p;
}

mpz_class NtermRecursive(mpz_class l, std::vector<size_t> primeIndices, int power, int term)
{
    uint64_t i = 0;

    while ((i < primeIndices.size()) && (primeIndices[i] != -1))
    {
        i += 1;
    }

    if (i == primeIndices.size())
    {
        mpz_class result;
        mpz_class div = l / product(primeIndices);
        mpz_pow_ui(result.get_mpz_t(), div.get_mpz_t(), power);
        return result;
    }

    mpz_class sum = 0;

    if (i == 0)
    {
        while (1)
        {
            primeIndices[i] += 1;
            if (primeList[primeIndices[i]] > (l / leastPrimeProduct(term - 1)))
                break;

            sum += NtermRecursive(l, primeIndices, power, term);
        }
        return sum;
    }

    while (1)
    {
        primeIndices[i] += 1;

        if ((i > 0) && (primeIndices[i] >= primeIndices[i - 1]))
            break;


        std::vector<size_t> primeSubIndices = std::vector<size_t>(primeIndices.begin(), primeIndices.begin() + i + 1);

        if (product(primeSubIndices) > l)
            break;

        sum += NtermRecursive(l, primeIndices, power, term);
    }
    return sum;
}


void Nterm(mpz_class l, int power, int term, mpz_class* p_ret)
{
    std::vector<size_t> primeIndices(term, -1);
    *p_ret = NtermRecursive(l, primeIndices, power, term);
    std::cout << "Term " << term << " done." << std::endl;
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
    if (l <= pow(10, 6))
        maxterm = 8;

    // Create a vector to store the threads
    std::vector<std::thread> threads;
    std::vector<mpz_class> rets(maxterm-1,0);
    // Create a variable to store the final sum
    mpz_class sum;

    short term = 1;
    short sign = -1;

    mpz_pow_ui(sum.get_mpz_t(), l.get_mpz_t(), power);
    while (term < maxterm)
    {
        // Create a new thread that runs the Nterm function
        std::thread t(Nterm, l, power, term, &rets[term-1]);
        // Add the thread to the vector of threads
        threads.push_back(std::move(t));
        term++;
    }

    // Wait for all threads to finish
    for (auto& t : threads)
    {
        t.join();
    }

    for (mpz_class ret : rets)
    {
        std::cout << ret << " ";
        sum += ret * sign;
        sign *= (-1);
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




int main()
{
    using std::cout;
    using std::endl;


    size_t L = std::pow(10,7);

    mpz_class L_(std::to_string(L));

    std::chrono::time_point<std::chrono::steady_clock> primestart = std::chrono::steady_clock::now();


    primeList = generatePrimes(L+1000);
    std::cout << "length of primeList: " << primeList.size() << std::endl;
    std::chrono::time_point<std::chrono::steady_clock> primeend = std::chrono::steady_clock::now();
    std::chrono::duration<double> prime_elapsed_seconds = primeend - primestart;
    std::cout << "getting the primelist: " << prime_elapsed_seconds.count() << " seconds" << std::endl;


    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    mpf_class result = NPioverL(L_, 2);
    cout << endl;
    cout << "result: " << std::setprecision(15) << result << endl;

    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Getting the result in: " << elapsed_seconds.count() << " seconds" << std::endl;





    
    system("pause");
    return 0;


}
