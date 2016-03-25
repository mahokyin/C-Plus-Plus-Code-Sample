// Name: Joe Ma
// Course: CSS 342

#include "longint.h"

using namespace std;

// Constructor
LongInt::LongInt(const string str) {
    int index = 0;

    // Handle the case if that's negative number
    if (str[index] == '-') {
        negative = true;
        index++;
    } else negative = false;

    // Input numbers to the queue
    for (; index < str.size(); index++)
        if (isdigit(str[index])) digits.addBack(str[index]);

    // Handle the invalid input by removing 0s
    remove0s();
}

LongInt::LongInt(const LongInt &rhs) {
    negative = rhs.negative;
    digits = rhs.digits;
}

LongInt::LongInt() {
    digits.addFront('0');
    negative = false;
}

// Destructor
LongInt::~LongInt( ) {
    negative = false;
    digits.clear();
}

// Input operator overloading
istream& operator>>(istream &input, LongInt &rhs) {
    // Assuming that user may have a chance to re-input the number again
    // It's neccessary to reset everything if it happens
    rhs.negative = false;
    rhs.digits.clear();

    // Create the string the receive the input from user
    string str;
    input >> str;

    // Initialize the index and set it as 0
    int index = 0;

    // Handle the case if that's negative number
    if (str[index] == '-') {
        rhs.negative = true;
        index++;
    } else rhs.negative = false;

    // Input numbers to the queue
    for (; index < str.size(); index++)
        if (isdigit(str[index])) rhs.digits.addBack(str[index]);

    // Handle the invalid input by removing 0s
    rhs.remove0s();

    return input;
}

// Output operator overloading
ostream& operator<<(ostream &output, const LongInt &rhs) {
    LongInt copy(rhs); // Make a copy using copy conctructor

    // Showing the value
    if (copy.negative) output << "-";
    while (!copy.digits.isEmpty())
        output << copy.digits.removeFront();

    return output;
}

// assignment operators
const LongInt& LongInt::operator=( const LongInt &rhs ) {
    negative = rhs.negative;
    digits = rhs.digits; // Using the assignment operator implemented in Deque class
    return *this;
}

void LongInt::remove0s( ) {
    // Handle the case if that's 0 or 000000000000, etc
    char currentFront = this->digits.getFront();
    if (currentFront == '0') {
        // Remove the 0 until the digit != 0
        for (; currentFront == '0' && !this->digits.isEmpty(); currentFront = this->digits.removeFront());
        // Need to push back the digit poped out before
        this->digits.addFront(currentFront);
        // To make sure -0 won't happen
        if (this->digits.size() == 1 && this->digits.getFront() == '0')
            this->negative = false;
    }
}

// Arithmetic binary operators +
LongInt LongInt::operator+( const LongInt &rhs ) const {
    int carry = 0; char digit;
    LongInt copyLhs(*this);
    LongInt copyRhs(rhs);

    // Situation checking
    // positive lhs + negative rhs means ans = lhs - rhs. You should call operator-.
    if (!copyLhs.negative && copyRhs.negative) {
        copyRhs.negative = false;
        return copyLhs - copyRhs;
    }
    // negative lhs + positive rhs means ans = rhs - lhs. You should call operator-.
    if (copyLhs.negative && !copyRhs.negative) {
        copyLhs.negative = false;
        return copyRhs - copyLhs;
    }

    // Procedure for calculation (+)
    int size = (copyLhs.digits.size() <= copyRhs.digits.size()) ? copyLhs.digits.size() : copyRhs.digits.size();

    // The program will initialize the object to 0, so we need to empty the queue
    LongInt longInt; longInt.digits.clear();

    for (int i = 0; i < size; i++) {
        int leftNum = CharToInt(copyLhs.digits.removeBack());
        int rightNum = CharToInt(copyRhs.digits.removeBack());
        digit = IntToChar(( leftNum + rightNum + carry ) % 10);
        longInt.digits.addFront(digit);
        carry = (leftNum + rightNum + carry) / 10;
    }

    // Handle the case like 5 + 5 = 10, 8 + 7 = 15, etc
    size = copyLhs.digits.size() - copyRhs.digits.size();
    if (size == 0) {
        // Handle the case if the input is 99 + 11, etc
        if (carry == 1) longInt.digits.addFront(IntToChar(carry));
        // Handle the situation if both of values are negative [-(lhs + rhs)]
        if (copyLhs.negative && copyRhs.negative) longInt.negative = true;
        return longInt;
    }

    // Handle the case like 33333333 + 8, 4353245245 + 4535, etc
    size = (size < 0) ? (size) * (-1) : size;
    for (int i = 0; i < size; i++) {
        if (copyLhs.digits.size() >= copyRhs.digits.size()) {
            int leftNum = CharToInt(copyLhs.digits.removeBack());
            digit = IntToChar(leftNum + carry);
            longInt.digits.addFront(digit);
            carry = (leftNum + carry) / 10;
        } else {
            int rightNum = CharToInt(copyRhs.digits.removeBack());
            digit = IntToChar(rightNum + carry);
            longInt.digits.addFront(digit);
            carry = (rightNum + carry) / 10;
        }
    }
    // Handle the case if the input is 999 + 1, etc
    if (carry == 1) longInt.digits.addFront(IntToChar(carry));

    // Handle the situation if both of values are negative [-(lhs + rhs)]
    if (copyLhs.negative && copyRhs.negative) longInt.negative = true;
    return longInt;
}


// Arithmetic binary operators -
LongInt LongInt::operator-( const LongInt &rhs ) const {
    int borrow = 0; char digit;
    LongInt copyLhs(*this);
    LongInt copyRhs(rhs);

    // positive lhs - negative rhs means ans = lhs + rhs
    if (!copyLhs.negative && copyRhs.negative) {
        copyLhs.negative = false; copyRhs.negative = false;
        return copyLhs + copyRhs;
    }

    // negative lhs - positive rhs means ans = -(lhs + rhs). This corresponds to the 4th case of operator+.
    if (copyLhs.negative && !copyRhs.negative) {
        copyLhs.negative = false; copyRhs.negative = false;
        LongInt ans = copyLhs + copyRhs;
        ans.negative = true;
        return ans;
    }

    // negative lhs - negative rhs means ans = rhs - lhs
    if (copyLhs.negative && copyRhs.negative) {
        copyLhs.negative = false; copyRhs.negative = false;
        return copyRhs - copyLhs;
    }

    // If rhs > lhs, then swap them plus the '-'
    if (copyRhs > copyLhs) {
        LongInt ans = copyRhs - copyLhs;
        ans.negative = true;
        return ans;
    }

    // Procedure for calculation (-)
    int size = (copyLhs.digits.size() <= copyRhs.digits.size()) ? copyLhs.digits.size() : copyRhs.digits.size();

    // The program will initialize the object to 0, so we need to empty the queue
    LongInt longInt; longInt.digits.clear();

    for (int i = 0; i < size; i++) {
        int leftNum = CharToInt(copyLhs.digits.removeBack());
        int rightNum = CharToInt(copyRhs.digits.removeBack());
        if (leftNum - rightNum < 0) {
            if (borrow == 0) {
                digit = IntToChar(10 + leftNum - rightNum);
                borrow++;
                longInt.digits.addFront(digit);
            } else {
                digit = IntToChar(10 + leftNum - rightNum - borrow);
                longInt.digits.addFront(digit);
            }
        } else {
            if (leftNum - rightNum - borrow < 0) digit = IntToChar(10 + leftNum - rightNum - borrow);
            else {
                digit = IntToChar(leftNum - rightNum - borrow);
                borrow = 0;
            }
            longInt.digits.addFront(digit);
        }
    }

    // Prevent the case like "045" happened
    longInt.remove0s();

    // If the size is 0, it means that the length of both value are the same
    // If the size is 0, the below statement won't be executed
    // because i = 0 < size = 0 --> false in for statement
    // If the size != 0, it means that the case would be like 53245245 - 24435, 55452245 - 4, etc
    size = copyLhs.digits.size() - copyRhs.digits.size();
    size = (size < 0) ? (size) * (-1) : size;
    for (int i = 0; i < size; i++) {
        if (copyLhs.digits.size() >= copyRhs.digits.size()) {
            int leftNum = CharToInt(copyLhs.digits.removeBack());
            if (leftNum - borrow < 0) digit = IntToChar(10 + leftNum - borrow);
            else {
                digit = IntToChar(leftNum - borrow);
                borrow = 0;
            }
            longInt.digits.addFront(digit);
        } else {
            int rightNum = CharToInt(copyRhs.digits.removeBack());
            if (rightNum - borrow < 0) digit = IntToChar(10 + rightNum - borrow);
            else {
                digit = IntToChar(rightNum - borrow);
                borrow = 0;
            }
            longInt.digits.addFront(digit);
        }
    }

    // Prevent the case like "045" happened
    longInt.remove0s();

    return longInt;
}


bool LongInt::operator>( const LongInt & rhs ) const {
    // Compare the the length of the number first, then compare the '-'
    if (digits.size() > rhs.digits.size()) {
        if (negative) return false;
        else return true;
    } else if (digits.size() < rhs.digits.size()) {
        if (rhs.negative) return true;
        else return false;
    } else if (digits.size() == rhs.digits.size()) {
        if (!negative && rhs.negative) return true;
        else if (negative && !rhs.negative) return false;
    }

    // Comparing digit by digit if those number having the same length
    LongInt copyLhs(*this);
    LongInt copyRhs(rhs);
    int leftNum, rightNum;
    while (!copyLhs.digits.isEmpty()) {
        leftNum = CharToInt(copyLhs.digits.removeFront());
        rightNum = CharToInt(copyRhs.digits.removeFront());
        if (!negative && !rhs.negative) {
            if (leftNum > rightNum) return true;
            else if (leftNum < rightNum) return false;
        } else {
            if (leftNum > rightNum) return false;
            else if (leftNum < rightNum) return true;
        }
    }

    // return false if the they're equal
    return false;
}

bool LongInt::operator==( const LongInt & rhs ) const {
    // Comparing the the length of the number fisrt
    if (digits.size() != rhs.digits.size()) return false;
    // Compareing the '-'
    if (negative != rhs.negative) return false;

    // Comparing digit by digit if those number having the same length
    LongInt copyLhs(*this);
    LongInt copyRhs(rhs);
    int leftNum, rightNum;
    while (!copyLhs.digits.isEmpty()) {
        leftNum = CharToInt(copyLhs.digits.removeFront());
        rightNum = CharToInt(copyRhs.digits.removeFront());
        if (leftNum != rightNum) return false;
    }

    // Return ture if they're equal
    return true;
}

bool LongInt::operator!=( const LongInt & rhs ) const {
    return !(*this == rhs);
}

bool LongInt::operator>=( const LongInt & rhs ) const {
    return (*this > rhs || *this == rhs);
}

bool LongInt::operator< ( const LongInt & rhs ) const {
    return !(*this >= rhs);
}

bool LongInt::operator<=( const LongInt & rhs ) const {
    return (*this < rhs) || (*this == rhs);
}

// Function for converting char to int
int LongInt::CharToInt(const char c) const {
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default: return 0;
    }
}

// Function for converting integer to char
char LongInt::IntToChar(const int c) const {
    switch (c) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        default: return '0';
    }
}
