#include <stdio.h>
#include <iostream>

struct Bnum;

Bnum Addition(Bnum*, Bnum*);
Bnum Subtraction(Bnum*, Bnum*);

struct Bnum {

	char sign;
	char* number;
	int size;

	Bnum() 
	{
		sign=0;
		number = NULL;
		size = 0;
	}

	void Get()
	{
		number = new char[5000];
		for (int i = 0; i < 5000; i++)
		{
			number[i] = NULL;
		}

		char c;
		int i = 0;
		while (true)
		{
			scanf_s("%c", &c,1);
			if ((i == 0 || c=='-'))
			{
				if (c == '-')
				{
					sign = '-';
					continue;
				}
				else
				{
					if (!iswspace(c))
					{
						if(sign!='-')sign = 0;

						number[i] = c;
						
						i++;

					}
				}
			}
			else
			{
				if (iswspace(c))break;

				number[i] = c;
				
				i++;
			}
		}

		size = i;

		char* num = (char*)realloc(number, (size + 3) * sizeof(char));

		if (num != NULL) number = num;
		else free(num);
	}

	void Print()
	{
		if (sign == '-')printf("%c", sign);
		for(int i=0;i<size;i++) printf("%c", number[i]);

		printf("\n");
	}
};

Bnum operator+(Bnum one, Bnum other)
{
	if (one.sign == other.sign) return Addition(&one, &other);
	else {
		//if (other.sign == '-')other.sign = 0;
		return Subtraction(&one, &other);
	}
}

Bnum operator-(Bnum one, Bnum other)
{
	if (one.sign == other.sign) return Subtraction(&one, &other);
	else {
		Bnum temp = Addition(&one, &other);
		temp.sign = (one.sign == '-') ? '-' : 0;
		return temp;
	}
}


bool WhichBigger(Bnum* one,Bnum* two)// zwraca true jezeli one nie jest mnieszy
{
	if (one->sign == two->sign)
	{
		bool ret;

		if (one->sign == '-') ret = false;
		else ret = true;

		if (one->size > two->size) return ret;
		else if (one->size < two->size)return (!ret);
		else// sa sobie rowne wiec trzeba sprawdzic ktora wieksza
		{
			for (int i = 0; i < one->size+1; i++)
			{
				if (one->number[i] != two->number[i])
				{
					if (one->number[i] > two->number[i])return ret;
					else return (!ret);
				}
			}

			return true;
		}
	}
	else
	{
		if (one->sign == '-')return false;
		else return true;
	}
}

Bnum Subtraction(Bnum* one, Bnum* two)
{
	Bnum tempo;
	bool change = false;

	if (one->sign == '-')
	{
		change = true;
		one->sign = 0;
	}

	if (two->sign == '-')
	{
		two->sign = 0;
	}
	
	if (WhichBigger(one, two))
	{
		int counter = 0;
		if (one->size >= two->size)counter = one->size;
		else counter = two->size;

		tempo.number = new char[counter + 3];
		tempo.size = counter;
		for (int i = 0; i < counter + 3; i++) tempo.number[i] = NULL;

		for (int i = 0; i < counter; i++)
		{
			tempo.number[counter - 1 - i] +=  (one->size - i > 0) ? one->number[one->size - 1 - i] : 0;
			tempo.number[counter - 1 - i] -= (two->size - i > 0) ? two->number[two->size - 1 - i] : 0;

			if (two->size - i > 0 && one->size - i > 0) tempo.number[counter - 1 - i] += '0';

			if(two->size-i > 0 && one->size-i < 0) tempo.number[counter - 1 - i] = 'c';//testy TU JEST BUG A TO NIE POWINNO BYC MOZLIWE

			if (tempo.number[counter - 1 - i] < '0')
			{
				tempo.number[counter -1 - i] += 10;
				tempo.number[counter - 2 - i]--;
			}
		}
	}
	else
	{
		tempo = Subtraction(two, one);
		change = change ? false: true;
	}

	int i = 0;
	while (true)
	{
		if (tempo.number[i] == '0' && tempo.size>1)
		{
			tempo.size--;

			for (int j = 0; j < tempo.size + 1; j++)
			{
				tempo.number[j] = tempo.number[j + 1];
			}

			tempo.number[tempo.size] = NULL;
		}
		else break;
	}


	if (change)tempo.sign = '-';

	return tempo; //powinno tworzy c nowa zmienna zeby ja potem zwrocic
}

Bnum Addition(Bnum* one, Bnum* two)
{
	Bnum tempo;
	int counter = 0;
	if (one->size >= two->size)counter = one->size;
	else counter = two->size;

	tempo.number = new char[counter +3];
	tempo.size = counter;
	if (one->sign == '-' && two->sign == '-') tempo.sign = '-';
	else tempo.sign = 0;

	for (int i = 0; i < counter + 3; i++)tempo.number[i] = NULL;

	for (int i = 0; i < counter; i++)
	{
		tempo.number[counter - 1 - i] += (one->size -1- i >= 0) ? one->number[one->size - 1 - i] : 0;
		tempo.number[counter - 1 - i] += (two->size -1- i >= 0) ? two->number[two->size - 1 - i] : 0;

		if (two->size-1 - i >= 0 && one->size-1-i >= 0 ) tempo.number[counter-1-i]-= '0';

		if (tempo.number[counter-1-i] > '9')
		{
			tempo.number[counter - 1 - i] -= 10;
			if (counter-1-i==0)
			{
				tempo.size++;
				for (int j = tempo.size - 1; j > 0; j--)
				{
					tempo.number[j] = tempo.number[j - 1];
				}

				tempo.number[0] = '1';
			}
			else tempo.number[counter - 2 - i]++;
		}
	}
	return tempo;
}


int StringToInt(char* c)
{
	int i = 0;
	while (!iswspace(*c) && *c>='0' && *c<='9')
	{
		i *= 10;
		i += *c - '0';
		scanf_s("%c", c,1);
	}

	return i;
}

void Control(Bnum *numbers,int n)
{
	char c=0;
	while(true)
	{
		scanf_s("%c", &c,1);

		if (c == '?')
		{
			for (int i = 0; i < n; i++)
			{
				numbers[i].Print();
			}
		}

		if (c == 'm')//maks min
		{
			scanf_s("%c", &c,1);
			if (c == 'a')//max
			{
				Bnum temp=numbers[0];

				for (int i = 1; i < n; i++)
				{
					if (WhichBigger(numbers+i, &temp))
					{
						temp = numbers[i];
					}
				}

				temp.Print();
			}

			if (c == 'i')//min
			{
				Bnum temp=numbers[0];

				for (int i = 1; i < n; i++)
				{
					if (WhichBigger(&temp,numbers+i))
					{
						temp = numbers[i];
					}
				}

				temp.Print();
			}

		}
			
		if (c <= '9' && c >= '0')//dodawanie odejmowanie
		{
			int i = 0, j = 0, k = 0;
			bool adding=true;
			
			i = StringToInt(&c);
			scanf_s("%c", &c,1);
			while (iswspace(c))scanf_s("%c", &c,1);
			scanf_s("%c", &c,1);
			while (iswspace(c))scanf_s("%c", &c,1);
			j = StringToInt(&c);
			scanf_s("%c", &c,1);
			while (iswspace(c))scanf_s("%c", &c,1);
			
			if (c == '+')adding = true;
			else adding = false;
			scanf_s("%c", &c,1);
			while (iswspace(c))scanf_s("%c", &c,1);
			k = StringToInt(&c);

			if (adding)numbers[i] = numbers[j] + numbers[k];
			else numbers[i] = numbers[j] - numbers[k];
		}

		if (c == 'q')
		{
			exit(0);
		}
	}
}

int main()
{
	long int n=0;
	scanf_s("%ld", &n);
	Bnum* numbers;

	numbers = (Bnum*)malloc((n+2)* sizeof(Bnum));

	for (int i = 0; i < n; i++)
	{
		numbers[i].Get();
	}

	Control(numbers,n);

	 free(numbers);
}
