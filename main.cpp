/* Bugs:

    1) For test problem #16, program sometimes crashes if I randomly shuffle the statements vector
       once in a while in attempt_proof().

    2) For problem #20, to show a contradiction, the program keeps wanting to use
       !Pv!Q and !(!Pv!Q). This works, but there's sometimes quicker ways. E.g., P and !P already
       known, but program then derives !Pv!Q to show contradiction.
            - Refer to page in notebook with big star.
   Ideas:

    1) Shuffle the statements vector in the attempt_proof() function sometimes. Could help
       if the AI is stuck somewhere.

    Notes:

    1) If recording a message for starting a supposition, make sure the first four chars of the
       text start with "Supp".
    2) Then when finishing the supposition, record the message as the supposition_failed or
       supposition_success const string vars. Do this specific recording at the very end of all
       other recordings for the supposition.
    3) Make sure all messages are stored in the steps_of_proof global vector.
    4) When getting a new statement, always make sure you call the remove_outside_brackets function
       on it.
*/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <time.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

const string supposition_failed = "Failed supposition";
const string supposition_success = "Successful supposition";
const string contradiction_string = "contradiction";

void remove_any_spaces(string& statement)
{
    string new_statement;

    for (char c: statement)
    {
        if (c != ' ')
        {
            new_statement += c;
        }
    }

    statement = new_statement;
}

void get_user_inputs(vector<string>& premises, string& conclusion)
{
    cout << "Once you've entered some statement(s), enter \"done\" for the last statement you've entered to be the";
    cout << " conclusion that gets proven.\n";
    cout << "Or, if you'd like to quit the program, enter \"done\" right now, for statement #1.\n";

    string user_input = "";
    for (int i = 1; user_input != "done"; ++i) {
        if (i > 1) {
            premises.push_back(user_input);
        }
        cout << "Enter statement #" << i << ": ";
        getline(cin, user_input);
    }

    if (!premises.empty()) {
        for (string& premise: premises) {
            remove_any_spaces(premise);
        }
        conclusion = premises[premises.size()-1];
        premises.pop_back();
    }
}

// Function returns -1 if there is no main operator.
int get_main_operator_index(const string& statement, bool look_for_certain_operator,
                            char certain_operator_to_look_for, bool exclude_certain_operator,
                            char certain_operator_to_exclude)
{
    if (!look_for_certain_operator && !exclude_certain_operator)
    {
        int main_operator_index_without_negation = get_main_operator_index(statement, false, ' ', true, '!');

        if (main_operator_index_without_negation != -1)
        {
            return main_operator_index_without_negation;
        }
    }

    int left_bracket_counter = 0;
    int right_bracket_counter = 0;

    for (int i = 0; i < statement.size(); i++)
    {
        char c = statement[i];

        if (c == '(')
        {
            left_bracket_counter ++;
        }

        if (c == ')')
        {
            right_bracket_counter ++;
        }

        if (left_bracket_counter == right_bracket_counter)
        {
            if (c == 'v' || c == '^' || c == '!' || c == '>' || c == '=')
            {
                if ((!look_for_certain_operator || c == certain_operator_to_look_for) &&
                    (!exclude_certain_operator || c != certain_operator_to_exclude))
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

void remove_outside_brackets(string& statement)
{
    if (statement.size() == 0)
    {
        throw runtime_error("empty string statement for remove_outside_brackets()");
    }

    if (get_main_operator_index(statement, false, ' ', false, ' ') == 0 && statement[0] == '!')
    {
        statement = statement.substr(1, statement.size()-1);

        remove_outside_brackets(statement);

        // Now add the '!' back in:

        if (statement.size() > 1)
        {
            statement = "(" + statement + ")"; // So that the negation applies to the whole statement.
        }

        statement = "!" + statement;
    }

    else
    {
        string old_version = statement;

        if (statement[0] == '(' && statement[statement.size()-1] == ')')
        {
            statement = statement.substr(1, statement.size() - 2);

            // Now make sure statement is okay:

            bool passed_open_bracket = false;

            bool passed_closed_bracket = false;

            int num_open_brackets = 0;

            int num_closed_brackets = 0;

            for (char c: statement)
            {
                if (c == '(')
                {
                    num_open_brackets++;

                    passed_open_bracket = true;
                }

                if (c == ')')
                {
                    num_closed_brackets++;

                    if (!passed_open_bracket)
                    {
                        statement = old_version;
                        return;
                    }

                    else if (num_closed_brackets >= num_open_brackets)
                    {
                        passed_open_bracket = false; // Since it's been closed by its corresponding
                                                     // closed bracket.
                    }
                }
            }

            if (passed_open_bracket || num_open_brackets != num_closed_brackets)
            {
                // Unclosed open bracket, or there aren't the same number of open and closed brackets:

                statement = old_version;
                return;
            }

            remove_outside_brackets(statement);
        }
    }
}

string get_left_statement(const string& statement, char main_operator)
{
    int main_operator_index = get_main_operator_index(statement, true, main_operator, false, ' ');

    string left_statement = statement.substr(0, main_operator_index);

    remove_outside_brackets(left_statement);

    return left_statement;
}

string get_right_statement(const string& statement, char main_operator)
{
    int main_operator_index = get_main_operator_index(statement, true, main_operator, false, ' ');

    string right_statement = statement.substr(main_operator_index + 1, (statement.size() - main_operator_index - 1));

    remove_outside_brackets(right_statement);

    return right_statement;
}

bool in_vector(const vector<string>& statements, const string& s)
{
    for (const string& current: statements)
    {
        if (s == current)
        {
            return true;
        }
    }

    return false;
}

bool attempt_contradiction(vector<string> statements, string additional_statement)
{
    // CONTINUE HERE:

    // Function returns true if a contradiction can be made using the statements.

    // Eee if anything the additional statement implies leads to a direct contradiction?

    return false;
}

bool contains(const string& statement, const string& text)
{
    for (int i = 0; i <= statement.size() - text.size(); i++)
    {
        if (statement.substr(i, text.size()) == text)
        {
            return true;
        }
    }

    return false;
}

vector<string> get_statements_containing_statement(const vector<string>& statements,
                                                   const string& statement)
{
    vector<string> selected_strings;

    for (string current: statements)
    {
        if (contains(current, statement))
        {
            selected_strings.push_back(current);
        }
    }

    return selected_strings;
}

bool is_a_conditional(const string& statement)
{
    return (get_main_operator_index(statement, true, '>', false, ' ')) != -1;

   //  return contains(statement, ">");
}

int index_of(const string& text, char c)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == c)
        {
            return i;
        }
    }

    throw runtime_error("char does not exist in string");
}

string get_stuff_after_character(const string& text, int index_of_character, bool eliminate_brackets)
{
   // int index_of_character = index_of(text, c);

    int starting_index = index_of_character + 1;

    string objective = text.substr(starting_index, text.size() - starting_index);

    if (eliminate_brackets)
    {
        remove_outside_brackets(objective);
    }

    return objective;
}

string get_stuff_before_character(const string& text, int index_of_character, bool eliminate_brackets)
{
  //  int index_of_character = index_of(text, c);

    int ending_index = index_of_character - 1;

    string objective = text.substr(0, ending_index + 1);

    if (eliminate_brackets)
    {
        remove_outside_brackets(objective);
    }

    return objective;
}

vector<string> get_statements_where_statement_is_a_consequent(const vector<string>& statements,
                                                              const string& statement)
{
    vector<string> chosen_statements;

    for (string current: statements)
    {
        if (is_a_conditional(current))
        {
            int main_operator_index = get_main_operator_index(current, true, '>', false, ' ');

            string consequent = get_stuff_after_character(current, main_operator_index, true);

            if (statement == consequent)
            {
                chosen_statements.push_back(current);
            }
        }
    }

    return chosen_statements;
}

vector<string> get_vector_without_string(const vector<string>& vec, const string& element)
{
    vector<string> replacement;

    for (string current: vec)
    {
        if (current != element)
        {
            replacement.push_back(current);
        }
    }

    return replacement;
}

bool is_proper_letter(char c)
{
    // by proper letter, I mean any letter except for 'v', since this is the disjunction.

    return (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) && c != 'v');
}

bool has_statement(const vector<string>& statements, const string& statement)
{
    for (string current: statements)
    {
        if (current == statement)
        {
            return true;
        }
    }

    return false;
}

bool does_negation_apply_to_whole_statement(string statement)
{
    if (statement[0] != '!')
    {
        return false;
    }

    statement = statement.substr(1, statement.size()-1);

    if (statement.size() == 1)
    {
        return true;
    }

    if (statement[0] != '(')
    {
        return false;
    }

    // Now see if this opening bracket is only closed at the very end of the statement.

    int open_bracket_counter = 1;
    int closed_bracket_counter = 0;

    for (int i = 1; i < statement.size(); i++)
    {
        if (statement[i] == '(')
        {
            open_bracket_counter ++;
        }

         else if (statement[i] == ')')
         {
             closed_bracket_counter ++;
         }

         if (open_bracket_counter == closed_bracket_counter)
         {
             return (i == statement.size()-1);
         }
    }

    throw runtime_error("Statement doesn't have the same number of open and closed brackets");
}

bool is_negation_of(string first_statement, string second_statement)
{
    // Return true if the first_statement is the negation of the second one.
    // In this case, this means the first_statement is the one with a '!' in front of it.

    if (first_statement[0] != '!')
    {
        return false;
    }

    // Check that the '!' in first_statement applies to the whole statement:

    if (!does_negation_apply_to_whole_statement(first_statement))
    {
        return false;
    }

    // Remove the beginning '!' from first_statement:

    first_statement = first_statement.substr(1, first_statement.size()-1);

    // Remove any outside brackets:

    remove_outside_brackets(first_statement);

    remove_outside_brackets(second_statement); // just to be safe.

    return (first_statement == second_statement);
}

bool any_contradictory_statements(const vector<string>& statements)
{
    for (int i = 0; i < statements.size(); i++)
    {
        for (int x = 0; x < statements.size(); x++)
        {
            if (is_negation_of(statements[i], statements[x]))
            {
                return true;
            }
        }
    }

    return false;
}

// Pre-condition: There must be a pair of contradictory statements.
vector<string> get_contradictory_statements(const vector<string>& statements)
{
    vector<string> result;

    for (int i = 0; i < statements.size(); i++)
    {
        for (int x = 0; x < statements.size(); x++)
        {
            if (is_negation_of(statements[i], statements[x]))
            {
                result.push_back(statements[i]);

                result.push_back(statements[x]);

                return result;
            }
        }
    }

    throw runtime_error("No contradictory statements");
}

string negate_by_adding_exclamation(const string& statement)
{
    string negated_statement;

    if (statement.size() == 1)
    {
        negated_statement = "!" + statement;
    }

    else
    {
        negated_statement = "!(" + statement + ")";
    }

    return negated_statement;
}

// Post-condition: If there isn't an existing negation applying to the statement, the empty string is returned.
string negate_by_removing_exclamation(const string& statement)
{
    string negated_statement = "";

    if (does_negation_apply_to_whole_statement(statement))
    {
        negated_statement = statement.substr(1, statement.size()-1);

        remove_outside_brackets(negated_statement);
    }

    return negated_statement;
}

int number_of_exclamation_marks_at_beginning(const string& statement)
{
    int exclamation_counter = 0;

    for (char c: statement)
    {
        if (is_proper_letter(c))
        {
            // c is a letter, so stop.

            break;
        }

        if (c == '!')
        {
            exclamation_counter ++;
        }
    }

    return exclamation_counter;
}

void remove_string_from_vec(vector<string>& vec, const string& element)
{
    vector<string> replacement;

    for (const string& current: vec)
    {
        if (current != element)
        {
            replacement.push_back(current);
        }
    }

    vec = replacement;
}

vector<string> remove_duplicates(const vector<string>& vec)
{
    vector<string> result;

    for (const string& current: vec)
    {
        if (!in_vector(result, current))
        {
            result.push_back(current);
        }
    }

    return result;
}

vector<string> get_primitive_statements(const vector<string>& statements)
{
    vector<string> primitive_statements;

    for (const string& current: statements)
    {
        for (char c: current)
        {
            if (is_proper_letter(c))
            {
                string s = "";

                s += c;

                primitive_statements.push_back(s);

                s = "!" + s;

                primitive_statements.push_back(s);
            }
        }
    }

    return remove_duplicates(primitive_statements);
}

bool only_implications_and_biconditionals(const vector<string>& statements)
{
    for (string current: statements)
    {
        int main_operator_index = get_main_operator_index(current, false, ' ', false, ' ');

        if (!(current[main_operator_index] == '>' || current[main_operator_index] == '='))
        {
            return false;
        }
    }

    return true;
}

int get_num_occurrences(const string& s, char c)
{
    int counter = 0;

    for (char current: s)
    {
        if (current == c)
        {
            counter ++;
        }
    }

    return counter;
}

bool any_equivalent_statement(const vector<string>& statements, const string& statement)
{
    // Function returns true if there is a statement that is logically equivalent with statement via
    // double negation introduction/removal.

    int num_exclamations_of_statement = get_num_occurrences(statement, '!');

    for (string current: statements)
    {
        if (current == statement)
        {
            return true;
        }

        // Now see if a double negation can be added/removed from current to make it like statement.

        int num_exclamations_of_current = get_num_occurrences(current, '!');

        if (num_exclamations_of_current > num_exclamations_of_statement)
        {
            current = negate_by_removing_exclamation(negate_by_removing_exclamation(current));

            if (current == statement)
            {
                return true;
            }
        }

        else if (num_exclamations_of_current < num_exclamations_of_statement)
        {
            current = negate_by_adding_exclamation(negate_by_adding_exclamation(current));

            if (current == statement)
            {
                return true;
            }
        }
    }

    return false;
}

bool same_elements(vector<string> first, vector<string> second)
{
    sort(first.begin(), first.end());

    sort(second.begin(), second.end());

    return (first == second);
}

void randomly_shuffle(vector<string>& vec)
{
    vector<string> old_vec = vec;

    vector<string> replacement;

    while (!vec.empty())
    {
        int index = rand() % vec.size();

        replacement.push_back(vec[index]);

        vec.erase(vec.begin() + index);
    }

    if (!same_elements(old_vec, replacement))
    {
        throw runtime_error("randomly_shuffle function has a bug");
    }

    vec = replacement;
}

vector<string> steps_of_proof;

string base_conclusion = ""; // For each problem, this is the overall conclusion trying to be shown.
                             // If it's proven when in a base scope (not in an undischarged supp), then
                             // even if the current conclusion isn't it, return true up the call stack,
                             // since the problem is solved.

bool solved_base_conclusion_in_base_scope = false;

bool attempt_proof(vector<string> statements, string conclusion, bool is_overall_goal_contradiction,
                   bool& showed_contradiction_instead_of_conclusion, int num_embedded_supps)
{
    // The "is_overall_goal_contradiction" parameter equals true if the purpose of proving the
    // conclusion is to show a contradiction.

    // So, even though the program is working towards inferring the conclusion, if a contradiction
    // happens to arise with two contradictory premises, the program can stop and return true.
        // Note that the main "focus" is on trying to show the conclusion, not going on tangents
        // to derive a contradiction.

    // If a contradiction is shown and the function returns, even though conclusion wasn't explicitly
    // the contradiction_string, then the fourth parameter is set to true. The reason is to let
    // the caller function know what happened (mainly just to print the appropriate thing to the screen
    // when showing the proof steps later on).

    // The 5th parameter, num_embedded_supps, is the number of embedded suppositions control is
    // currently in.
        // Ex: It equals 2 if control is in a supposition within a supposition.

  /*  if (rand() % 10 == 1)
    {
        randomly_shuffle(statements);
    } */

    for (string& current: statements)
    {
        remove_outside_brackets(current);
    }

    remove_outside_brackets(conclusion);

  //  steps_of_proof.push_back("CONC trying to prove: " + conclusion);

    string message = "";

    // Return true if the conclusion could be proven, false otherwise.

    // Note that conclusion could be a statement, or the string "contradiction".
        // In this case, the goal is to show/prove a contradiction with the statements I have.

    if (conclusion == contradiction_string || is_overall_goal_contradiction)
    {
        if (any_contradictory_statements(statements))
        {
            vector<string> contradictory_statements = get_contradictory_statements(statements);

            message = "Found a contradiction with " + contradictory_statements[0] +
                      " and " + contradictory_statements[1] + "\n";

            steps_of_proof.push_back(message);

            if (is_overall_goal_contradiction && conclusion != contradiction_string)
            {
                showed_contradiction_instead_of_conclusion = true;
            }

            return true;
        }
    }

    // Will do this check after updating the statements with any new statements below.


    // Try deriving new statements from the existing statements, and then try to prove
    // the conclusion again.

    // While I'm going through the existing statements, I might as well check if one of them
    // equals the original_conclusion and conclusion.

    int old_num_statements = statements.size();

    for (int i = 0; i < statements.size(); i++)
    {
        string current = statements[i];

        if (current == base_conclusion && num_embedded_supps == 0)
        {
            message = "Proved " + base_conclusion;
            steps_of_proof.push_back(message);

            solved_base_conclusion_in_base_scope = true;

            return true;
        }

        if (current == conclusion)
        {
            message = "Reit " + conclusion + "\n";
            steps_of_proof.push_back(message);

            return true;
        }

        int main_operator_index = get_main_operator_index(current, false, ' ', false, ' ');

        if (current[main_operator_index] == '^')
        {
            string left_statement = get_left_statement(current, '^');

            string right_statement = get_right_statement(current, '^');

            if (!in_vector(statements, left_statement))
            {
                statements.push_back(left_statement);

                message = "Proved " + left_statement + " by conjunction eliminiation\n";

                steps_of_proof.push_back(message);
            }

            if (!in_vector(statements, right_statement))
            {
                statements.push_back(right_statement);

                message = "Proved " + right_statement + " by conjunction eliminiation\n";

                steps_of_proof.push_back(message);
            }

            // Adding stuff to a vector I'm iterating through isn't an error, because one of the statements
            // in a conjunction could be a conjunction itself, so I'd want to revisit it on its own
            // and break it down as well.
        }

        if (current[main_operator_index] == '>')
        {
            string antecedent = get_stuff_before_character(current, main_operator_index, true);

            if (in_vector(statements, antecedent))
            {
                string consequent = get_stuff_after_character(current, main_operator_index, true);

                if (!in_vector(statements, consequent))
                {
                    statements.push_back(consequent);

                    message = "Proved " + consequent + " by Modus Ponens on " + current + "\n";
                    steps_of_proof.push_back(message);
                }
            }
        }

        if (current[main_operator_index] == '=')
        {
            string left_side = get_stuff_before_character(current, main_operator_index, false);
            string right_side = get_stuff_after_character(current, main_operator_index, false);

            string forward_implication = left_side + '>' + right_side;
            string backward_implication = right_side + '>' + left_side;

            if (!in_vector(statements, forward_implication))
            {
                statements.push_back(forward_implication);

                message = "Proved " + forward_implication + " by biconditional elimination\n";
                steps_of_proof.push_back(message);
            }

            if (!in_vector(statements, backward_implication))
            {
                statements.push_back(backward_implication);

                message = "Proved " + backward_implication + " by biconditional elimination\n";

                steps_of_proof.push_back(message);
            }
        }
    }

    if (statements.size() != old_num_statements)
    {
        if (attempt_proof(statements, conclusion, is_overall_goal_contradiction,
                          showed_contradiction_instead_of_conclusion, num_embedded_supps))
        {
            return true;
        }
    }

    if (conclusion == contradiction_string)
    {
        // I can't show a contradiction immediately, (already checked this at the beginning of this function),
        // so instead go through each known statement, negate it, and see if it can be proven.

        for (string current_statement: statements)
        {
            string negation1 = negate_by_removing_exclamation(current_statement);
            string negation2 = negate_by_adding_exclamation(current_statement);

            if (negation1 != "" && attempt_proof(statements, negation1, true,
                                                 showed_contradiction_instead_of_conclusion,
                                                 num_embedded_supps))
            {
                if (solved_base_conclusion_in_base_scope)
                {
                    return true;
                }

                message = "Showed a contradiction\n";

                steps_of_proof.push_back(message);

                return true;
            }

            if (number_of_exclamation_marks_at_beginning(negation2) < 3 &&
                attempt_proof(statements, negation2, true, showed_contradiction_instead_of_conclusion,
                              num_embedded_supps))
            {
                if (solved_base_conclusion_in_base_scope)
                {
                    return true;
                }

                message = "Showed a contradiction\n";

                steps_of_proof.push_back(message);

                return true;
            }
        }
    }

    int main_operator_index_of_conclusion = get_main_operator_index(conclusion, false, ' ', false, ' ');

    if (main_operator_index_of_conclusion == -1 && conclusion != contradiction_string)
    {
        // No main operator, and not "contradiction", so the conclusion is a single variable.
        // To prove it, first see if it's in my statements vector:

        if (in_vector(statements, conclusion))
        {
            message = "Showed " + conclusion + " by Reit\n";

            steps_of_proof.push_back(message);

            return true;
        }

        // Couldn't prove it directly, so see if I can do it via contradiction:

        string negated_conclusion = negate_by_adding_exclamation(conclusion);

        if (attempt_contradiction(statements, negated_conclusion))
        {
            message = "Proved " + conclusion + " via contradiction\n";

            steps_of_proof.push_back(message);

            return true; // proved the conclusion, since negating it implies a contradiction.
        }
    }

    char main_operator = conclusion[main_operator_index_of_conclusion];

    if (main_operator == '^' || main_operator == 'v')
    {
        string left_statement = "";

        string right_statement = "";

        if (main_operator == '^')
        {
            left_statement = get_left_statement(conclusion, '^');

            right_statement = get_right_statement(conclusion, '^');
        }

        else if (main_operator == 'v')
        {
            left_statement = get_left_statement(conclusion, 'v');

            right_statement = get_right_statement(conclusion, 'v');
        }

        bool first_reporter = false;
        bool second_reporter = false;
        // These will be sent by reference as the fourth parameter in the calls below.

        bool first_proof = attempt_proof(statements, left_statement,
                                         is_overall_goal_contradiction, first_reporter,
                                         num_embedded_supps);

        if (solved_base_conclusion_in_base_scope)
        {
            return true;
        }

        if (is_overall_goal_contradiction && first_reporter)
        {
            message = "Showed a contradiction\n";

            steps_of_proof.push_back(message);

            showed_contradiction_instead_of_conclusion = true;

            return true;
        }

        if (main_operator == 'v' && first_proof)
        {
            message = "Proved " + conclusion + " by disjunction introduction\n";

            steps_of_proof.push_back(message);

            return true;
        }

        bool second_proof = attempt_proof(statements, right_statement,
                                          is_overall_goal_contradiction, second_reporter,
                                          num_embedded_supps);

        if (solved_base_conclusion_in_base_scope)
        {
            return true;
        }

        if (is_overall_goal_contradiction && second_reporter)
        {
            message = "Showed a contradiction\n";

            steps_of_proof.push_back(message);

            showed_contradiction_instead_of_conclusion = true;

            return true;
        }

        if (main_operator == '^')
        {
            if (first_proof && second_proof)
            {
                message = "Proved " + conclusion + " by conjunction introduction\n";

                steps_of_proof.push_back(message);

                return true;
            }
        }

        else if (main_operator == 'v')
        {
            if (first_proof || second_proof)
            {
                message = "Proved " + conclusion + " by disjunction introduction\n";

                steps_of_proof.push_back(message);

                return true;
            }
        }
    }

    else if (main_operator == '!')
    {

    }

    else if (main_operator == '>')
    {
        // Suppose the antecedent, and then try to prove the consequent:

        string antecedent = get_stuff_before_character(conclusion, main_operator_index_of_conclusion, true);

        string consequent = get_stuff_after_character(conclusion, main_operator_index_of_conclusion, true);

        vector<string> statements_with_supp = statements;
        statements_with_supp.push_back(antecedent);

        message = "Supp " + antecedent + "...\n";

        steps_of_proof.push_back(message);

        bool throwaway; // fourth parameter doesn't matter on this call, since I know for sure that
                        // a contradiction isn't the overall goal.

        if (attempt_proof(statements_with_supp, consequent, false, throwaway, num_embedded_supps + 1))
        {
            message = "Proved " + conclusion + " via proof by supposition\n";
            steps_of_proof.push_back(message);
            steps_of_proof.push_back(supposition_success);

            return true;
        }

        else
        {
            steps_of_proof.push_back(supposition_failed);
        }
    }

    else if (main_operator == '=')
    {
        string left_side = get_stuff_before_character(conclusion, main_operator_index_of_conclusion, false);
        string right_side = get_stuff_after_character(conclusion, main_operator_index_of_conclusion, false);

        string forward_implication = left_side + '>' + right_side;
        string backward_implication = right_side + '>' + left_side;

        remove_outside_brackets(forward_implication);
        remove_outside_brackets(backward_implication);
        // Above two lines are for just in case.

        bool first_reporter = false;
        bool second_reporter = false;
        // These will be sent by reference as the fourth parameter in the calls below.

        bool first_proof = attempt_proof(statements, forward_implication,
                                         is_overall_goal_contradiction, first_reporter,
                                         num_embedded_supps);

        if (solved_base_conclusion_in_base_scope)
        {
            return true;
        }

        if (is_overall_goal_contradiction && first_reporter)
        {
            message = "Showed a contradiction\n";

            steps_of_proof.push_back(message);

            showed_contradiction_instead_of_conclusion = true;

            return true;
        }

        bool second_proof = attempt_proof(statements, backward_implication,
                                          is_overall_goal_contradiction, second_reporter,
                                          num_embedded_supps);

        if (solved_base_conclusion_in_base_scope)
        {
            return true;
        }

        if (is_overall_goal_contradiction && second_reporter)
        {
            message = "Showed a contradiction\n";

            steps_of_proof.push_back(message);

            showed_contradiction_instead_of_conclusion = true;

            return true;
        }

        if (first_proof && second_proof)
        {
            message = "Proved conclusion by biconditional introduction\n";

            steps_of_proof.push_back(message);

            return true;
        }
    }

    // At this point, see if the conclusion appears in some form in our statements:

    if (in_vector(statements, conclusion))
    {
        message = "Reit " + conclusion + "\n";
        steps_of_proof.push_back(message);

        return true;
    }

    vector<string> statements_where_conclusion_is_a_consequent =
    get_statements_where_statement_is_a_consequent(statements, conclusion);

    if (!statements_where_conclusion_is_a_consequent.empty())
    {
        // So there is at least one statement where the conclusion acts as the consequent.
        // See if we can prove it by proving the antecedent of one of these statements:

        for (string current_conditional: statements_where_conclusion_is_a_consequent)
        {
            int main_operator_index_of_conditional = get_main_operator_index(current_conditional, true, '>',
                                                                             false, ' ');

            string antecedent = get_stuff_before_character(current_conditional,
                                                           main_operator_index_of_conditional, true);

            if (attempt_proof(statements, antecedent, is_overall_goal_contradiction,
                              showed_contradiction_instead_of_conclusion, num_embedded_supps))
            {
                if (solved_base_conclusion_in_base_scope)
                {
                    return true;
                }

                if (is_overall_goal_contradiction && showed_contradiction_instead_of_conclusion)
                {
                    message = "Showed a contradiction\n";

                    steps_of_proof.push_back(message);

                    return true;
                }

                else
                {
                    message = "Proved " + conclusion + " by Modus Ponens " + "\n";

                    steps_of_proof.push_back(message);

                    return true;
                }
            }
        }
    }

    // Try a proof by contradiction on the conclusion (assuming right now a proof by contradiction is already
    // trying to be shown):

    if (conclusion != contradiction_string)
    {
        // Negate conclusion and then try to prove via contradiction.

        // Note that if the conclusion already starts with a '!', there are two ways to negate it:
            // Add a '!' before
            // Remove the '!'.
            // In this case, I'll attempt two separate proofs by contradiction.

        string negated_conclusion1 = ""; // Will have the '!' added before.
        string negated_conclusion2 = ""; // Will have the existing '!' removed, if applicable.

        negated_conclusion1 = negate_by_adding_exclamation(conclusion);

        negated_conclusion2 = negate_by_removing_exclamation(conclusion);
        // If a negation doesn't apply to the conclusion, negated_conclusion2 stays as the empty string.

        if (negated_conclusion2 != "" && !any_equivalent_statement(statements, negated_conclusion2))
        {
            vector<string> statements_with_negated_conclusion = statements;

            statements_with_negated_conclusion.push_back(negated_conclusion2);

            message = "Supp " + negated_conclusion2 + "/!Int\n";
            steps_of_proof.push_back(message);

            bool throwaway; // since the conclusion is a contradiction, so the 4th parameter is irrelevant.

            if (attempt_proof(statements_with_negated_conclusion, contradiction_string, true, throwaway,
                              num_embedded_supps + 1))
            {
                message = "Proved the conclusion " + conclusion + " via a proof by contradiction, by assuming " +
                          "the negated conclusion: " + negated_conclusion2 + "\n";
                steps_of_proof.push_back(message);
                steps_of_proof.push_back(supposition_success);

                return true;
            }

            else
            {
                steps_of_proof.push_back(supposition_failed);
            }
        }

        if (!any_equivalent_statement(statements, negated_conclusion1) &&
            number_of_exclamation_marks_at_beginning(negated_conclusion1) < 3)
        {
            vector<string> statements_with_negated_conclusion = statements;

            statements_with_negated_conclusion.push_back(negated_conclusion1);

            message = "Supp " + negated_conclusion1 + "/!Elim\n";
            steps_of_proof.push_back(message);

            bool throwaway;

            if (attempt_proof(statements_with_negated_conclusion, contradiction_string, true, throwaway,
                              num_embedded_supps + 1))
            {
                message = "Proved the conclusion " + conclusion + " via a proof by contradiction, by assuming " +
                          "the negated conclusion: " + negated_conclusion1 + "\n";
                steps_of_proof.push_back(message);
                steps_of_proof.push_back(supposition_success);

                return true;
            }

            else
            {
                steps_of_proof.push_back(supposition_failed);
            }
        }
    }

    // The last thing I'll try is seeing if I can do a proof by cases on a premise.
    // I didn't do this when I was going through the premises before, because it's more like a "last resort".
    // There's no need to do this with every disjunction statement if I don't have to.

    // Besides, before I was adding more statements to my statements vector. Here with a proof by cases,
    // I wouldn't be adding more statements technically (just assuming and calling this function via recursion).

    for (int i = 0; i < statements.size(); i++)
    {
        string current = statements[i];

        int main_operator_index = get_main_operator_index(current, false, ' ', false, ' ');

        if (current[main_operator_index] == 'v')
        {
            // Try a proof by cases:

            string left_side = get_stuff_before_character(current, main_operator_index, true);
            string right_side = get_stuff_after_character(current, main_operator_index, true);

            if (!has_statement(statements, left_side) && !has_statement(statements, right_side))
            {
                // If either the left or right side was already a known statement, the program would just
                // prove the conclusion based off that, instead of having to show the conclusion follows
                // from either the left or right side.

                vector<string> statements_with_left = statements;
                statements_with_left.push_back(left_side);
                remove_string_from_vec(statements_with_left, current);

                vector<string> statements_with_right = statements;
                statements_with_right.push_back(right_side);
                remove_string_from_vec(statements_with_right, current);

                message = "Supp " + left_side + " vElim\n";
                steps_of_proof.push_back(message);

                bool first_reporter = false;
                bool second_reporter = false;

                if (attempt_proof(statements_with_left, conclusion, is_overall_goal_contradiction,
                                  first_reporter, num_embedded_supps + 1))
                {
                    message = "Supp " + right_side + " vElim\n";

                    steps_of_proof.push_back(message);

                    if (attempt_proof(statements_with_right, conclusion, is_overall_goal_contradiction,
                                      second_reporter, num_embedded_supps + 1))
                    {
                        // Two success messages, since two suppositions succeeded.

                        // Before writing this success message though, I have to decide what to say
                        // for the proof justification to the user. It depends on if a contradiction
                        // as the overall goal was proven in 0, 1, or both of the proofs.

                        if (is_overall_goal_contradiction && (first_reporter || second_reporter))
                        {
                            if (first_reporter && second_reporter)
                            {
                                message = "Proved a contradiction by disjunction elimination on " +
                                          current;

                                showed_contradiction_instead_of_conclusion = true;
                            }

                            else if (first_reporter && !second_reporter)
                            {
                                message = "Showed a contradiction for the left side of " + current +
                                          ": " + left_side + ", and I proved " + conclusion +
                                          " for the right side: " + right_side +
                                          "\nThis will also lead to a contradiction, which will finish " +
                                          "the disjunction elimination";
                            }

                            else if (second_reporter && !first_reporter)
                            {
                                message = "Showed a contradiction for the right side of " + current +
                                          ": " + right_side + ", and I proved " + conclusion +
                                          " for the left side: " + left_side +
                                          "\nThis will also lead to a contradiction, which will finish " +
                                          "the disjunction elimination";
                            }

                            else
                            {
                                throw runtime_error("Something went wrong...");
                            }

                            steps_of_proof.push_back(message);
                        }

                        else
                        {
                            message = "Proved " + conclusion + " by disjunction elimination on " +
                                      current + "\n";
                            steps_of_proof.push_back(message);
                        }

                        steps_of_proof.push_back(supposition_success);
                        steps_of_proof.push_back(supposition_success);

                        return true;
                    }

                    else
                    {
                        steps_of_proof.push_back(supposition_failed);
                        steps_of_proof.push_back(supposition_failed);

                        // Two failure messages since I had two messages for displaying both
                        // the left and right suppositions. I don't want to display either now.
                    }
                }

                else
                {
                    steps_of_proof.push_back(supposition_failed);
                }
            }
        }
    }

    // It might be the case that no primitive statement is known per se, and that I only have certain
    // compound statements (i.e., just implications and/or biconditionals).

    // In this scenario, I want to assume some primitive statement appearing in one of the compound
    // statements, and then try to show how this leads to a contradiction (so that I can prove
    // its negation exists).

    // If I can do this, then call this function again with the updated statements vector, and the
    // same conclusion.

    if (only_implications_and_biconditionals(statements))
    {
        vector<string> primitive_statements = get_primitive_statements(statements);

        for (string current_primitive_statement: primitive_statements)
        {
            string justification = ", !Int";

            if (current_primitive_statement[0] == '!')
            {
                justification = ", !Elim";
            }

            steps_of_proof.push_back("Supp " + current_primitive_statement + justification);

            vector<string> statements_with_addition = statements;
            statements_with_addition.push_back(current_primitive_statement);

            bool throwaway;

            if (attempt_proof(statements_with_addition, contradiction_string, true, throwaway,
                              num_embedded_supps + 1))
            {
                // The proof by contradiction worked, so add the primitive statement's negated form
                // to the statements vector.

                string new_statement = "";

                if (current_primitive_statement[0] == '!')
                {
                    new_statement = negate_by_removing_exclamation(current_primitive_statement);
                }

                else
                {
                    new_statement = negate_by_adding_exclamation(current_primitive_statement);
                }

                statements.push_back(new_statement);

                message = "Proved " + new_statement + " by proof by contradiction";

                steps_of_proof.push_back(message);

                steps_of_proof.push_back(supposition_success);

                if (attempt_proof(statements, conclusion, is_overall_goal_contradiction,
                                  showed_contradiction_instead_of_conclusion, num_embedded_supps))
                {
                    if (solved_base_conclusion_in_base_scope)
                    {
                        return true;
                    }

                    if (is_overall_goal_contradiction && showed_contradiction_instead_of_conclusion)
                    {
                        steps_of_proof.push_back("Showed a contradiction");
                    }

                    else
                    {
                        steps_of_proof.push_back("Proved " + conclusion);
                    }

                    return true;
                }
            }

            else
            {
                steps_of_proof.push_back(supposition_failed);
            }
        }
    }

    return false;
}

void eliminate_newlines_for_each_string(vector<string>& vec)
{
    for (string& current: vec)
    {
        while (current[current.size()-1] == '\n')
        {
            current = current.substr(0, current.size()-1);
        }
    }
}

bool do_previous_suppositions_succeed(const vector<string>& vec, int i, int num_previous_suppositions)
{
    int num_future_suppositions = 0;

    for (; i < vec.size(); i++)
    {
        string future_string = vec[i];

        if (future_string.substr(0,4) == "Supp" || future_string.substr(0,4) == "supp")
        {
            num_future_suppositions ++;
        }

        if (future_string == supposition_success || future_string == supposition_failed)
        {
            if (num_future_suppositions > 0)
            {
                num_future_suppositions --;
                // don't care if the future supposition failed/succeeded.
            }

            else
            {
                // Not in a future supposition, so one of the previous supposition(s) has
                // been finished:

                if (future_string == supposition_failed)
                {
                    return false;
                }

                else if (future_string == supposition_success)
                {
                    num_previous_suppositions --;

                    if (num_previous_suppositions == 0)
                    {
                        // Made it out of any previous suppositions successfully.

                        return true;
                    }
                }
            }
        }
    }
    throw runtime_error("Control reached the end of the do_previous_suppositions_succeed function.");
}

void print_steps_of_proof()
{
    vector<string> trimmed_steps;

    int supposition_counter = 0;

    eliminate_newlines_for_each_string(steps_of_proof);

    for (int i = 0; i < steps_of_proof.size(); i++)
    {
        string current = steps_of_proof[i];

        if (current.substr(0,4) == "Supp" || current.substr(0,4) == "supp")
        {
            supposition_counter ++;
        }

        if (current == supposition_failed || current == supposition_success)
        {
            supposition_counter --; // since out of the supposition now.
        }

        else if (supposition_counter == 0)
        {
            trimmed_steps.push_back(current);
        }

        else
        {
            /* The current string is embedded in one or more suppositions.

            // Only print it to the screen if it was useful in solving the problem:
                1) For each of the supposition(s) it is currently in, they all are successful.
                2) For any suppositions being introduced after it, these don't matter.
            */

            if (do_previous_suppositions_succeed(steps_of_proof, i+1, supposition_counter))
            {
                trimmed_steps.push_back(current);
            }
        }
    }

    // Now print out the trimmed steps:

    for (int i = 0; i < trimmed_steps.size(); i++)
    {
        string current = trimmed_steps[i];

        if (!(i > 0 && current == trimmed_steps[i-1]))
        {
            cout << current << "\n";
        }
    }
}

bool attempt_proof_call(const vector<string>& statements, const string& conclusion)
{
    steps_of_proof.clear();

    base_conclusion = conclusion;

    solved_base_conclusion_in_base_scope = false;

    bool throwaway;

    bool try_to_solve = attempt_proof(statements, conclusion, (conclusion == contradiction_string),
                                      throwaway, 0);
        // For the third parameter ("is_overall_goal_contradiction"), this is only the case
        // right now if the explicit goal is to show a contradiction.

    print_steps_of_proof();

    return try_to_solve;
}

int main()
{
    srand(time(NULL));
    vector<string> statements;
    string conclusion = "";

    while (true) {
        get_user_inputs(statements, conclusion);

        if (statements.empty()) {
            break;
        }

        cout << "\n";

        bool solved = attempt_proof_call(statements, conclusion);

        if (solved)
        {
            cout << "\nSolved the problem!\n";
        }

        else
        {
            cout << "Could not solve the problem.\n";
        }

        cout << "\n\n";

        statements.clear();

        conclusion = "";
    }
}

