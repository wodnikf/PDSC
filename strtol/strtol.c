#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>

int char_to_int_value(char c, int base)
{
	if (c >= 'A' && c <= 'Z')
	{
		return 10 + c - 'A';
	}
	else if (c >= 'a' && c <= 'z')
	{
		return 10 + c - 'a';
	}
	else if (isdigit(c))
	{
		return c - '0';
	}
	return -1; // Invalid character
}

void error_endPtr_setter(const char *nPtr, char **endPtr, int i, int base)
{
	bool loop = true;
	int temp;
	while (loop)
	{
		temp = char_to_int_value(nPtr[i], base);
		if (temp == -1)
		{
			loop = false;
			break;
		}
		if (temp >= base)
		{
			loop = false;
			break;
		}
		i++;
	}
	if (endPtr)
	{
		*endPtr = (char *)nPtr + i;
	}
}

void skip_whitespaces(const char *nPtr, int *help)
{
	while (isspace(nPtr[*help]))
	{
		(*help)++;
	}
}

void get_sign(const char *nPtr, int *help, bool *is_negative)
{
	if (nPtr[*help] == '-')
	{
		*is_negative = true;
		(*help)++;
	}
	else if (nPtr[*help] == '+')
	{
		(*help)++;
	}
}

void handle_zero_base(const char *nPtr, int *help, int *help2, int *base)
{
	if (!*base)
	{
		if (nPtr[*help] == '0' && tolower(nPtr[*help + 1]) == 'x')
		{
			*base = 16;
			(*help) += 2;
			(*help2) = *help - 1;
		}
		else if (nPtr[*help] == '0')
		{
			*base = 8;
			(*help)++;
			(*help2) = *help;
		}
		else if (isdigit(nPtr[*help]))
		{
			*base = 10;
		}
		else
		{
			errno = EINVAL;
			return;
		}
	}
}

void check_hex_oct_base(const char *nPtr, int *help, int *help2, int base)
{
	if (base == 16 && nPtr[*help] == '0' && tolower(nPtr[*help + 1]) == 'x')
	{
		(*help) += 2;
		*help2 = (*help) - 1;
	}
	else if (base == 8 && nPtr[*help] == '0')
	{
		(*help)++;
		*help2 = (*help);
	}
}

bool check_base_condition(int base)
{
	if (base < 2 || base > 36)
	{
		errno = EINVAL;
		return false;
	}
	return true;
}

long strtol(const char *nPtr, char **endPtr, int base)
{
	long number_value = 0;
	int help = 0;
	int help2 = 0;
	bool is_negative = false;
	bool is_empty = true;

	// Skipping whitespaces
	skip_whitespaces(nPtr, &help);

	// Sign
	get_sign(nPtr, &help, &is_negative);

	// Base
	handle_zero_base(nPtr, &help, &help2, &base);

	check_hex_oct_base(nPtr, &help, &help2, base);

	if (!check_base_condition(base))
	{
		return 0;
	}

	while (true)
	{
		int temp = char_to_int_value(nPtr[help], base);

		// Base condition
		if (temp == -1 || temp >= base)
		{
			break;
		}

		if (is_negative)
		{
			if (number_value < (LONG_MIN + temp) / base)
			{
				errno = ERANGE;
				error_endPtr_setter(nPtr, endPtr, help, base);
				return LONG_MIN;
			}
			number_value = number_value * base - temp;
		}
		else
		{
			if (number_value > (LONG_MAX - temp) / base)
			{
				errno = ERANGE;
				error_endPtr_setter(nPtr, endPtr, help, base);
				return LONG_MAX;
			}
			number_value = number_value * base + temp;
		}

		help++;
		is_empty = false;
	}

	if (is_empty && endPtr)
	{
		*endPtr = (char *)nPtr + help2;
	}
	else if (endPtr)
	{
		*endPtr = (char *)nPtr + help;
	}

	return number_value;
}
