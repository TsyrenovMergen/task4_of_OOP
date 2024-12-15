#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <iterator>
#include <sstream>

using namespace std;
template<class Ch, class Tr, size_t I, typename... Args>
struct tuple_printer
{
    static void print(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
    {
        // printing elements of the tuple before I-th element (instantiation 'recursion' call)
        tuple_printer<Ch, Tr, I - 1, Args...>::print(os, t);
        if (I < sizeof...(Args))
            os << ",";
        // printing I-th element
        os << std::get<I>(t);
    }
};

//partial specialization (I==0) for printing firt element and finishing 'recursion'
template<class Ch, class Tr, typename... Args>
struct tuple_printer<Ch, Tr, 0, Args...>
{
    static void print(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
    {
        os << get<0>(t);
    }
};

//partial specialization (I==-1) to make empty tuple printing work correctly
template<class Ch, class Tr, typename... Args>
struct tuple_printer<Ch, Tr, -1, Args...>
{
    static void print(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
    {
    }
};

template<class Ch, class Tr, typename... Args>
auto operator<<(basic_ostream<Ch, Tr>& os, const tuple<Args...>& t)
{
    os << "(";
    tuple_printer<Ch, Tr, sizeof...(Args) - 1, Args...>::print(os, t);
    os << ")";
}
template <size_t I, typename... Args>
struct tuple_inputer
{
    static void input(tuple <Args...>& t, string* arr)
    {
        tuple_inputer<I - 1, Args...>::input(t, arr);
        istringstream string_stream(arr[I]);
        string_stream >> get<I>(t);
        string_stream.clear();
    }
};
template <typename... Args>
struct tuple_inputer<0, Args...>
{
    static void input(tuple <Args...>& t, string* arr)
    {
        istringstream string_stream(arr[0]);
        string_stream >> get<0>(t);
        string_stream.clear();
    }
};
template <typename... Args>
struct tuple_inputer<-1, Args...>
{
    static void input(tuple <Args...>& t, string* arr)
    {
    }
};
template <typename... Args>
void parse_tuple(tuple<Args...>& t, string* arr)
{
    tuple_inputer<sizeof...(Args) - 1, Args...>::input(t, arr);
}
template <typename... Args> class Iterator;
template <typename... Args> class CSVParser {
public:
    CSVParser(char symbol_escape, char column_separator, char line_separator)
    {
        this->sym_escape = symbol_escape;
        this->column_sep = column_separator;
        this->line_sep = line_separator;
        this->is_end = false;
        this->cur_sym = new char[2];
        this->cur_sym[0] = '0';
        this->line = 0;
        this->column = 0;
        this->maxindex = sizeof...(Args);
        this->tokens = new string[this->maxindex];
        this->is_prev_char_escape = false;
        while (this->cur_sym[0] != '\0')
        {
            cin.read(this->cur_sym, 1);
            if (cin.gcount() == 0)
            {
                this->is_end = true;
            }
            this->is_cur_char_escape = this->cur_sym[0] == this->sym_escape;
            this->is_cur_char_column_sep = this->cur_sym[0] == this->column_sep;
            this->is_cur_char_line_sep = this->cur_sym[0] == this->line_sep;
            if (is_second_char_escape && is_cur_char_escape == false)
            {
                throw "need second char escape\n";
            }
            if (is_second_char_escape && is_cur_char_escape)
            {
                is_second_char_escape = false;
            }
            if (is_prev_char_escape == false && is_cur_char_column_sep)
            {
                this->column += 1;
            }
            if (is_prev_char_escape == false && this->is_cur_char_line_sep)
            {
                parse_tuple(this->t, this->tokens);
                break;
            }
            if (is_prev_char_escape == true && this->is_cur_char_line_sep || this->is_end)
            {
                this->tokens[this->column].push_back(this->line_sep);
                this->is_second_char_escape = true;
            }
            if (is_prev_char_escape == true && this->is_cur_char_column_sep)
            {
                this->tokens[this->column].push_back(this->column_sep);
                this->is_second_char_escape = true;
            }
            if (is_prev_char_escape == true && this->is_cur_char_escape)
            {
                this->tokens[this->column].push_back(this->sym_escape);
                this->is_second_char_escape = true;
            }
            if (this->is_prev_char_escape == false && this->is_cur_char_column_sep == false && this->is_cur_char_line_sep == false && this->is_cur_char_escape == false)
            {
                this->tokens[this->column].push_back(this->cur_sym[0]);
            }
            is_prev_char_escape = is_cur_char_escape;
        }
    }
    tuple <Args...>* next()
    {
        this->column = 0;
        this->line += 1;
        if (this->is_end)
        {
            return NULL;
        }
        for (unsigned int i = 0; i < this->maxindex; i++)
        {
            this->tokens[i] = "";
        }
        while (this->cur_sym[0] != '\0')
        {
            cin.read(this->cur_sym, 1);
            if (cin.gcount() == 0)
            {
                this->is_end = true;
            }
            this->is_cur_char_escape = this->cur_sym[0] == this->sym_escape;
            this->is_cur_char_column_sep = this->cur_sym[0] == this->column_sep;
            this->is_cur_char_line_sep = this->cur_sym[0] == this->line_sep;
            if (is_second_char_escape && is_cur_char_escape == false)
            {
                throw "need second char escape\n";
            }
            if (is_second_char_escape && is_cur_char_escape)
            {
                is_second_char_escape = false;
            }
            if (is_prev_char_escape == false && is_cur_char_column_sep)
            {
                this->column += 1;
            }
            if (is_prev_char_escape == false && this->is_cur_char_line_sep || is_end)
            {
                parse_tuple(this->t, this->tokens);
                break;
            }
            if (is_prev_char_escape == true && this->is_cur_char_line_sep)
            {
                this->tokens[this->column].push_back(this->line_sep);
                this->is_second_char_escape = true;
            }
            if (is_prev_char_escape == true && this->is_cur_char_column_sep)
            {
                this->tokens[this->column].push_back(this->column_sep);
                this->is_second_char_escape = true;
            }
            if (is_prev_char_escape == true && this->is_cur_char_escape)
            {
                this->tokens[this->column].push_back(this->sym_escape);
                this->is_second_char_escape = true;
            }
            if (this->is_prev_char_escape == false && this->is_cur_char_column_sep == false && this->is_cur_char_line_sep == false && this->is_cur_char_escape == false)
            {
                this->tokens[this->column].push_back(this->cur_sym[0]);
            }
            is_prev_char_escape = is_cur_char_escape;
        }
        return &(this->t);
    }
    Iterator<Args...> begin()
    {
        return Iterator<Args...>(&(this->t), this);
    }
    Iterator<Args...> end()
    {
        return Iterator<Args...>(NULL, this);
    }
private:
    tuple <Args...> t;
    string* tokens;
    unsigned long line;
    unsigned long column;
    unsigned long maxindex;
    char* cur_sym;
    char sym_escape;
    char column_sep;
    char line_sep;
    bool is_prev_char_escape;
    bool is_cur_char_escape;
    bool is_cur_char_line_sep;
    bool is_cur_char_column_sep;
    bool is_second_char_escape;
    bool is_end;
};
template <typename... Args> class Iterator
{
public:
    Iterator(tuple <Args...>* curr, CSVParser<Args...>* point) :current(curr)
    {
        this->pointer = point;
    }
    Iterator& operator ++()
    {
        this->current = pointer->next();
        return *this;
    }
    tuple<Args...>& operator *()
    {
        return *current;
    }
    bool operator ==(const Iterator& other)
    {
        return current == other.current;
    }
    bool operator !=(const Iterator& other)
    {
        return !(*this == other);
    }
private:
    tuple<Args...>* current;
    CSVParser <Args...>* pointer;
};
int main()
{
    char symbol_escape = '"';
    char column_separator = ',';
    char line_separator = '\n';
    CSVParser<int, string> parser(symbol_escape, column_separator, line_separator);
    for (tuple<int, string> rs : parser) {
        cout << rs;
        cout << endl;
    }
    return 0;
}