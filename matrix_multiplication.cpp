/*date: 2016.12.10-21
 *author: Vladislav Mambetov
 */

#include <stdio.h> //printf();
#include <iostream> //cout();
#include <termios.h> //struct termios;
#include <unistd.h> //tcgetattr(), tcsetattr();
#include <time.h> //time(NULL);
#include <stdlib.h> //rand(), srand();

#define full_clrscr() printf("\e[1;1H\e[2J") //cursor to pos.1,1; clear screan;
#define cursorof() printf("\e[?25l")
#define cursoron() printf("\e[?25h")
#define show_title(lab_5) printf("\e]0;Matrix multiplication\007")


using namespace std;

//for main_menu
void display_main_menu(int main_selected_item);
int main_menu_new_position(int current_position, int key, int quantity_itmes);

//for edit
int getkey();
void window(int iterim_left, int iterim_top, int iterim_width, \
	int iterim_hight); //80x24
void gotoxy(int x, int y);
void set_text_color(int color);
void set_background_color(int color);
void clrscr();
void full_clrscr_plus();

//case 0
int **get_matrix(int iterim_row, int iterim_column);
void fill_matrix_manually(int** matrix, int iterim_row, int iterim_column);
void fill_matrix_randomly(int** matrix, int iterim_row, int iterim_column);

//case 1
int **get_multiplication_matrixs(int** matrix_a, int** matrix_b, \
	int iterim_len_row_A, int iterim_len_column_A_row_B, \
	int iterim_len_column_B);
void print_matrix(int** iterim_matrix, int iterim_row, int iterim_column);

//case 2
int **copy_matrix(int** iterim_matrix, int iterim_row, int iterim_column);
void sorting_matrix(int **matrix, int iterim_len_row, int iterim_len_column);

//case 3
void change_language();
void display_list_languages(int iterim_pos_list_language, \
	string iterim_list_language[]);
int list_languages_new_position(int current_position, int key, \
	int quantity_itmes);

//case 4
void record_to_file(int **matrix, int n, int m, char *puth);

//case 5
void reading_file_data(int **&matrix, int &n, int &m, char *puth);

//window parameters
int 
	window_top = 0,
	window_right = 0,
	window_bottom = 0,
	window_left = 0,
	window_hight = 0,
	window_width = 0;

//windows const
const int
	WINDOW_FULL_WIDTH = 80,
	WINDOW_FULL_HIGHT = 24;

const int
	ESC = 9,
	ENT = 10;

bool
	flag_new_position = true,
	flag_exec = false;

bool 
	flag_case_0 = false,
	flag_case_1 = false;

int 
	previos_character,
	previos_position_item;

//languages
int
	INTERFACE_LANGUAGE = 0;

const int
	quantity_languages = 3;

string main_menu[quantity_languages][5] = {
	//English language -- default
	{
		"Enter the matrixs",
		"Output matrixs",
		"Sorting array",
		"Change the language",
		"Exit"
	},
	//Russian language
	{
		"Ввести массивы",
		"Вывести матрици",
		"Сортировать матрицу",
		"Изменить язык",
		"Выход"
	},
	//Ukrainian language
	{
		"Ввести матрицi",
		"Вивести матрицi",
		"Сортувати матрицю",
		"Змiнити мову",
		"Вихiд"
	}
};


int main() {

	string text_case_0[quantity_languages][5] = {
		//English language -- default
		{
			"Enter the matrix sizes (1..10)",
			"Quantity rows of matrix A: ",
			"Quantity columns of matrix A and rows of matrix B: ",
			"Quantity columns of matrix B: ",
			"How to fill in the matrix (r - randomly, m - manually): "
		},
		//Russian language
		{
			"Ввести размеры матриц (1..10)",
			"Количество рядков матрицы А: ",
			"Количество столбцов матрицы А и рядков матрицы Б: ",
			"Количество столбцов матрицы Б: ",
			"Как заполнить матрицу (r - рандомно, m - вручную): "
		},
		//Ukrainian language
		{
			"Ввести розмiри матриць (1..10)",
			"Кiлькiсть рядкiв матрицi А: ",
			"Кiлькiсть столбцiв матрицi А i рядкiв матрицi Б: ",
			"Кiлькiсть столбцiв матрицi Б: "
			"Як заповнити матриню (r - рандомно, m - вручну)"
		}
	};

	show_title(lab_5);

	int
		main_selected_item = 0,
		main_key,
		main_quantity_items = 7;

	int
		len_row_A,
		len_column_A_row_B,
		len_column_B;

	int** matrix_a = 0;
	int** matrix_b = 0;
	int** matrix_c = 0;
	int** copy_matrix_c;

	cursorof();

	full_clrscr_plus();
	/*
	for (int i = 0; i < 13; ++i)
	{
		full_clrscr();
	}
	*/

	while (true) {

		if (flag_new_position) {
			window(2, 2, 15, 60);
			display_main_menu(main_selected_item);
			window(0, 0, 0, 0);
		}

		main_key = getkey();

		if (main_key == ESC) {

			main_selected_item = main_quantity_items - 1;
			main_key = ENT;

			window(2, 2, 15, 60);
			display_main_menu(main_selected_item);
			window(0, 0, 0, 0);

		} else {

			main_selected_item = main_menu_new_position(main_selected_item, \
			main_key, main_quantity_items);
		}

		if (main_key == ENT) {

			window(0, 1, 0, -1);
			clrscr();

			switch (main_selected_item) {

				case 0: {

					cursoron();
					window(1, 1, -1, -1);

					gotoxy(1, 1);
					cout << text_case_0[INTERFACE_LANGUAGE][0] << '\n';

					do {
						gotoxy(1, 3);
						cout << string(window_width, ' ');

						gotoxy(1, 3);
						cout << text_case_0[INTERFACE_LANGUAGE][1];
						cin >> len_row_A;
					} while (len_row_A < 1 || len_row_A > 9);

					do {
						gotoxy(1, 4);
						cout << string(window_width, ' ');

						gotoxy(1, 4);
						cout << text_case_0[INTERFACE_LANGUAGE][2];
						cin >> len_column_A_row_B;
					} while (len_column_A_row_B < 1 || len_column_A_row_B > 9);

					do {
						gotoxy(1, 5);
						cout << string(window_width, ' ');

						gotoxy(1, 5);
						cout << text_case_0[INTERFACE_LANGUAGE][3];
						cin >> len_column_B;
					} while (len_column_B < 1 || len_column_B > 9);

					matrix_a = get_matrix(len_row_A, len_column_A_row_B);
					matrix_b = get_matrix(len_column_A_row_B, \
						len_column_B);

					full_clrscr_plus();

					while (true) {

						gotoxy(1, 1);
						cout << string(window_width, ' ');

						gotoxy(1, 1);
				 		cout << text_case_0[INTERFACE_LANGUAGE][4];
						char r_or_h;
						cin >> r_or_h;

						switch (r_or_h) {

							case 'r' :
							case 'R' : {

								fill_matrix_randomly(matrix_a, len_row_A, \
									len_column_A_row_B);
								fill_matrix_randomly(matrix_b, \
									len_column_A_row_B, len_column_B);

								break;
							}
							case 'm' :
							case 'M' : {

								full_clrscr_plus();

								window(1, 1, -1, -1);
								gotoxy(1, 1);
								cout << "Matrix A";

								window(1, 3, -1, -1);
								fill_matrix_manually(matrix_a, \
									len_row_A, len_column_A_row_B);
								
								full_clrscr_plus();

								window(1, 1, -1, -1);
								gotoxy(1, 1);
								cout << "Matrix B";

								window(1, 3, -1, -1);
								fill_matrix_manually(matrix_b, \
									len_column_A_row_B, len_column_B);

								break;
							}

								default : continue;
						}

						break; //everything is filled correctly;
					}

					flag_case_0 = true;
					cursorof();
					getchar();
					break;
				}

				case 1: {
					if(flag_case_0) {

						matrix_c = get_multiplication_matrixs(matrix_a, \
							matrix_b, len_row_A, \
							len_column_A_row_B, len_column_B);

						full_clrscr_plus();
						cout << "Matrix A:";
						cout << "\n\n";
						print_matrix(matrix_a, len_row_A, len_column_A_row_B);
						getkey();

						full_clrscr_plus();
						cout << "Matrix B:";
						cout << "\n\n";
						print_matrix(matrix_b, len_column_A_row_B, \
							len_column_B);
						getkey();

						full_clrscr_plus();
						cout << "Matrix C:";
						cout << "\n\n";
						print_matrix(matrix_c, len_row_A, len_column_B);
						getkey();

						flag_case_1 = true;
					}
					break;
				}

				case 2: {
					if(flag_case_1) {
						
						copy_matrix_c = copy_matrix(matrix_c, len_row_A, \
							len_column_B);
						
						sorting_matrix(copy_matrix_c, len_row_A, len_column_B);

						window(1, 1, -1, -1);
						full_clrscr_plus();
						cout << "Sorted Matrix C:";
						cout << "\n\n";
						print_matrix(copy_matrix_c, len_row_A, len_column_B);
						getkey();
					}
					break;
				}

				case  3:
					window(2, 2, 20, 10);
					change_language();
					break;

				case 4: {
					if (flag_case_1) {

						record_to_file(matrix_a, len_row_A, \
							len_column_A_row_B, "data_1.txt");
						record_to_file(matrix_b, len_column_A_row_B, \
							len_column_B, "data_2.txt");
						record_to_file(matrix_c, len_row_A, len_column_B, \
							"data_3.txt");


						full_clrscr_plus();
						cout << "Record...";
						getkey();
					}

					break;
				}

				case 5: {

					//reading_file_data(matrix_a, matrix_b, matrix_c, \
						*len_row_A, *len_column_A_row_B, *len_column_B);

					reading_file_data(matrix_a, len_row_A, len_column_A_row_B, \
						"data_1.txt");
					reading_file_data(matrix_b, len_column_A_row_B, \
						len_column_B, "data_2.txt");
					reading_file_data(matrix_c, len_row_A, len_column_B, \
						"data_3.txt");
					
					full_clrscr_plus();
					cout << "Open...";
					getkey();

					flag_case_0 = true;
					flag_case_1 = true;

					break;
				}

				case 6:
					window(0, 0, 0, 0);
					clrscr();
					cursoron();
					return 0;
			}

			window(0, 1, 0, -1);
			clrscr();
			flag_new_position = true;
		}
	}



	for (int i = 0; i < len_row_A; i++) {
		free(matrix_a[i]);
	}
	free(matrix_a);

	for (int i = 0; i < len_column_A_row_B; i++) {
		free(matrix_b[i]);
	}
	free(matrix_b);

	for (int i = 0; i < len_row_A; i++) {
		free(matrix_c[i]);
	}
	free(matrix_c);

	for (int i = 0; i < len_row_A; i++) {
		free(copy_matrix_c[i]);
	}
	free(copy_matrix_c);

	return -1;
}


void display_main_menu(int main_selected_item) {

	gotoxy(1, 1);
	(main_selected_item == 0) ? set_text_color(3) : set_text_color(5);
	cout << main_menu[INTERFACE_LANGUAGE][0];

	gotoxy(1, 2);
	(main_selected_item == 1) ? set_text_color(3) : set_text_color(5);
	cout << main_menu[INTERFACE_LANGUAGE][1];

	gotoxy(1, 3);
	(main_selected_item == 2) ? set_text_color(3) : set_text_color(5);
	cout << main_menu[INTERFACE_LANGUAGE][2];

	gotoxy(1, 4);
	(main_selected_item == 3) ? set_text_color(3) : set_text_color(5);
	cout << main_menu[INTERFACE_LANGUAGE][3];

	gotoxy(1, 5);
	(main_selected_item == 4) ? set_text_color(3) : set_text_color(5);
	cout << "Record to file";

	gotoxy(1, 6);
	(main_selected_item == 5) ? set_text_color(3) : set_text_color(5);
	cout << "Reading a file";

	gotoxy(1, 8);
	(main_selected_item == 6) ? set_text_color(3) : set_text_color(5);
	cout << main_menu[INTERFACE_LANGUAGE][4];

	set_text_color(5);
}

int getkey() {

	struct termios 
		oldt, 
		newt;
	
	int key;

	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	key = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	return key;
}

int main_menu_new_position(int current_position, int key, \
	int quantity_itmes) {

	flag_new_position = false;

	if (flag_exec) {

		switch (key) {

			case 65:
			case 53:
				(current_position == 0) ? current_position = \
					quantity_itmes - 1 : current_position -= 1;
				break;

			case 66:
			case 54:
				current_position += 1;
				current_position %= quantity_itmes;
				break;
		}

		flag_new_position = true;
	}

	if (previos_character != 91 && 49 <= key && key <= 48 + quantity_itmes) {

		(key - 49 == current_position) ? : flag_new_position = true;

		current_position = key - 49;
	}

	if (previos_character == 27 && key == 91) {

		current_position = previos_position_item;
		flag_exec = true;

	} else {

		flag_exec = false;
	}

	previos_character = key;
	previos_position_item = current_position;

	if (key == 27) {

		current_position = quantity_itmes - 1;
		flag_new_position = true;
	}

	return current_position;
}

void window(int iterim_left, int iterim_top, int iterim_width_or_rignt, \
	int iterim_hight_or_bottom) {

	window_top = iterim_top;
	window_left = iterim_left;

	if (iterim_width_or_rignt > 0) {
		window_width = iterim_width_or_rignt;
		window_right = WINDOW_FULL_WIDTH - iterim_width_or_rignt - iterim_left;
	} else {
		window_width = WINDOW_FULL_WIDTH + iterim_width_or_rignt - iterim_left;
		window_right = - iterim_width_or_rignt;
	}

	if (iterim_hight_or_bottom > 0) {
		window_hight = iterim_hight_or_bottom;
		window_bottom = WINDOW_FULL_HIGHT - iterim_hight_or_bottom - iterim_top;
	} else {
		window_hight = WINDOW_FULL_HIGHT + iterim_hight_or_bottom - iterim_top;
		window_bottom = - iterim_hight_or_bottom;
	}
}

void gotoxy(int x, int y) {
	
	printf("\e[%i;%iH", window_top + y, window_left + x);
}

void set_text_color(int color){

	printf("\e[3%im", color);
}

void set_background_color(int color) {

	printf("\e[4%im", color);
}

void clrscr() {

	for (int i = 0; i < window_hight -1; ++i) {

		gotoxy(1, i + 1);
		cout << string(window_width, ' ') << '\n';
	}
	cout << string(window_width, ' ');

	gotoxy(1, 1);
}

void full_clrscr_plus() {

	int
		copy_window_left = window_left,
		copy_window_top = window_top,
		copy_window_width = window_width,
		copy_window_hight = window_hight;

	window(0, 0, 0, 0);
	clrscr();

	set_text_color(0);
	gotoxy(1, 1);
	for (int u = 0; u < 80; u++) {
		cout << '*';
	}
	gotoxy(1, 24);
	for (int u = 0; u < 80; u++) {
		cout << '*';
	}

	window(copy_window_left, copy_window_top, \
		copy_window_width, copy_window_hight);

	set_text_color(5);

	gotoxy(1, 1);
}

int **get_matrix(int iterim_row, int iterim_column) {
	
	int **iterim_array = (int**) malloc(iterim_row * sizeof(int*));

	for (int u = 0; u < iterim_row; u++) {
		iterim_array[u] = (int*) malloc(iterim_column * sizeof(int));
	}

	return iterim_array;
}

void fill_matrix_manually(int** matrix, int iterim_row, int iterim_column) {

	//gotoxy(1, 1);

	for (int u1 = 0; u1 < iterim_row; u1++) {

		for (int u2 = 0; u2 < iterim_column; u2++) {

			gotoxy(1, 1);
			clrscr();
			printf("Matrix[%i][%i]: ", u1, u2);
			cin >> matrix[u1][u2];
		}
	}
}

void fill_matrix_randomly(int** matrix, int iterim_row, int iterim_column) {

	srand(time(NULL));

	for (int i = 0; i < iterim_row; ++i) {
		for (int j = 0; j < iterim_column; ++j) {
			matrix[i][j] = rand() % 20 - 10;
		}
	}
}

int **get_multiplication_matrixs(int** matrix_a, int** matrix_b, \
	int iterim_len_row_A, int iterim_len_column_A_row_B, \
	int iterim_len_column_B) {

	int **matrix_result = get_matrix(iterim_len_row_A, iterim_len_column_B);

	for(int i = 0; i < iterim_len_row_A; i++) {

		for(int j = 0; j < iterim_len_column_B; j++) {

			matrix_result[i][j] = 0;
		}
	}

	for (int i_1 = 0; i_1 < iterim_len_row_A; ++i_1) {

		for (int i_2 = 0; i_2 < iterim_len_column_B; ++i_2) {

			for (int i_3 = 0; i_3 < iterim_len_column_A_row_B; ++i_3) {

				matrix_result[i_1][i_2] += matrix_a[i_1][i_3] \
				* matrix_b[i_3][i_2];
			}
		}
	}

	return matrix_result;
}

void print_matrix(int** iterim_matrix, int iterim_row, int iterim_column) {

	for (int u1 = 0; u1 < iterim_row; u1++) {

		for (int u2 = 0; u2 < iterim_column; u2++) {

			//printf("Matrix[%i][%i] = ", u1, u2);
			//cout << iterim_matrix[u1][u2] << '\t';

			cout 
				<< "\t" 
				<< (iterim_matrix[u1][u2] >= 0?" ":"") 
				<< iterim_matrix[u1][u2];
		}

		cout << '\n';
	}
}

int **copy_matrix(int **iterim_matrix, int iterim_row, int iterim_column) {

	int **matrix_result = (int**) malloc (iterim_row * sizeof(int*));

	for (int u1 = 0; u1 < iterim_row; u1++) {

		matrix_result[u1] = (int *) malloc(iterim_column * sizeof(int));

		for (int u2 = 0; u2 < iterim_column; u2++) {

			matrix_result[u1][u2] = iterim_matrix[u1][u2];
		}
	}

	return matrix_result;
}

void sorting_matrix(int **matrix, int iterim_len_row, int iterim_len_column) {

	int c;

	for (int uu = 0; uu < iterim_len_row; uu++) {

		for (int i = 0; i < iterim_len_column - 1; i++) {

			for (int j = iterim_len_column - 2; j >= i; j--) {

				if (matrix[uu][j] > matrix[uu][j+1]) {

					c = matrix[uu][j];
					matrix[uu][j] = matrix[uu][j+1];
					matrix[uu][j+1] = c;
				}

			}
		}
	}

	for (int uu = 0; uu < iterim_len_column; uu++) {

		for (int i = 0; i < iterim_len_row - 1; i++) {

			for (int j = iterim_len_row - 2; j >= i; j--) {

				if (matrix[j][uu] > matrix[j+1][uu]) {

					c = matrix[j][uu];
					matrix[j][uu] = matrix[j+1][uu];
					matrix[j+1][uu] = c;
				}

			}
		}
	}
}

void change_language() {

	string list_languages[quantity_languages] = {

			"English",
			"Русский",
			"Украïнська"
	};

	int pos_list_language = INTERFACE_LANGUAGE;
	int key_list_language;

	clrscr();

	flag_new_position = true;

	while (true) {

		if (flag_new_position) display_list_languages(pos_list_language, \
			list_languages);

		key_list_language = getkey();
		
		if (key_list_language == ESC) break;
		if (key_list_language == ENT) {
			INTERFACE_LANGUAGE = pos_list_language;
			break;
		}
		
		pos_list_language = list_languages_new_position(pos_list_language, \
			key_list_language, quantity_languages);
	}
}

void display_list_languages(int iterim_pos_list_language, \
	string iterim_list_language[]) {

	for (int i = 0; i < quantity_languages; i++) {

		gotoxy(1, 1 + i);
		(iterim_pos_list_language == i) ? set_text_color(3) : set_text_color(5);
		cout << iterim_list_language[i];
	
		if (INTERFACE_LANGUAGE == i) {
			set_text_color(4);
			cout << " -- now";
		} else {
			cout << "       ";
		};
	}

	set_text_color(5);
}

int list_languages_new_position(int current_position, int key, \
	int quantity_itmes) {

	flag_new_position = true;

	switch (key) {

		case 65:
		case 53:
			(current_position == 0) ? current_position = \
				quantity_itmes - 1 : current_position -= 1;
			break;

		case 66:
		case 54:
			current_position += 1;
			current_position %= quantity_itmes;
			break;

		default:
			flag_new_position = false;
	}

	return current_position;
}

void record_to_file(int **matrix, int n, int m, char *puth) {

	FILE* file = fopen(puth, "w");

	fwrite(&n, sizeof(int), 1, file);
	fwrite(&m, sizeof(int), 1, file);

	for(int i=0; i < n; i++)
		fwrite(matrix[i], sizeof(int), m, file);

	fclose(file);
}

void reading_file_data(int **&matrix, int &n, int &m, char *puth) {

	FILE* file = fopen(puth, "r");

	if (matrix != 0) {
		for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);

	}

	fread(&n, sizeof(int), 1, file);
	fread(&m, sizeof(int), 1, file);

	matrix = get_matrix(n, m);

	for(int i=0; i < n; i++)
		fread(matrix[i], sizeof(int), m, file);

	fclose(file);
}