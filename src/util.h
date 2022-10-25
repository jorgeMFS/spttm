/**
    util.h
    Purpose: Utility functions for the program.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include <stddef.h>
#include <array>
#include <cmath>
#include <limits>
#include "turingMachine.h"

/**
    Returns integer base raised to the power of an integer exponent.

    @param base. int of the base.
    @param exp. int of the exponent.
    @return int base raised to the power of an int exponent.
*/
template <typename T>
T ipow(T base, T exp) {
    
    if (exp == 0) {
      return 1;
    }
    if (exp == 1) {
      return base;
    }
    T result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}

std::ostream& bold_on(std::ostream& os);
std::ostream& bold_off(std::ostream& os);
std::ostream& underlined_on(std::ostream& os);
std::ostream& cyan_on(std::ostream& os);
std::ostream& green_on(std::ostream& os);
std::ostream& red_on(std::ostream& os);
void set_output(const std::string& name, const double& value);
void set_output_units(const std::string& name, const double& value, const std::string& units);
void draw_line();

double loss_to_probability(double current_loss);

unsigned int compute_max_patience(double current_loss, unsigned int default_patience);

// Returns random boolean (0,1)
bool randomBool();

/** Calculate the number of Turing machines with the increase in number of states.
 * @param max_number_states the number of states cardinality
 */
void tm_growth_with_cardinality(size_t max_number_states);


void tm_fl_cardinality(size_t states, size_t alphabet_size);


// This function finds power of a number x 
void power(int x, int n);

//Compute_average_of_vector




/**  Returns TmId of the total number of Turing machines for a number of states and alphabet size.
 * @param states the state cardinality
 * @param alphabet_size the alphabet's size
*/
TmId get_tm_cardinality(unsigned int states, unsigned int alphabet_size);



/**  Returns cast of size_t to unsigned int.
 * @param size_t value
*/
unsigned int convert_szt_to_ui( size_t value );



/* This function multiplies x  with the number represented by res[]. 
 res_size is size of res[] or number of digits in the number represented by res[]. This function uses simple school mathematics for multiplication. 
This function may value of res_size and returns the new value of res_size */
int multiply(int x, int res[], int res_size);



/// Look-up table for log2
template <typename T, size_t precision>
class Log2Lut {
    std::array<T, precision + 1> lut;
    T m;

    public:
    Log2Lut()
    : m(precision)
    {
        /// for numerical stability, assume log(0) = -FLT_MAX
        this->lut[0] = std::numeric_limits<T>::lowest();
        for (size_t i = 1; i <= precision; i++) {
            T x = i;
            x /= this->m;
            this->lut[i] = std::log2(x);
        }
    }

    T log2(T x) const {
        auto i = static_cast<size_t>(x * m);
        return this->lut.at(i);
    }
};

extern Log2Lut<double, 1000000> DEFAULT_LOG2_LUT;

template <typename T, size_t max_value>
class Log2CardLut {
    std::array<T, max_value> lut;
    T m;

    public:
    Log2CardLut()
    : m(max_value)
    {
        /// for numerical stability, assume log(0) = -FLT_MAX
        this->lut[0] = std::numeric_limits<T>::lowest();
        for (size_t i = 1; i <= max_value; i++) {
            this->lut[i] = std::log2(i);
        }
    }

    T log2(unsigned int x) const {
        auto i = static_cast<size_t>(x);
        return this->lut.at(i);
    }
};

extern Log2CardLut<double, 100000> DEFAULT_LOG2_CARD_LUT;

