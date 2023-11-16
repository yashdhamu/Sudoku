#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "generate.h"
#include "loadgame.h"
#include "solve_game.h"

GtkWidget  *no[25][25], *solution[25][25];

/*
 *window_close : closes the previous window
 */
void window_close(GtkWidget *widget, gpointer data){
	gtk_grab_remove (GTK_WIDGET (data));
	gtk_widget_destroy (GTK_WIDGET(data));
}

/*
 *window_show : shows the previos window after closing the previous one
 */
void window_show(GtkWidget *widget, gpointer data){
	gtk_widget_show_all(GTK_WIDGET(data));
}

/*
 * peek_two : allows us to see solutiin for the 2x2 sudoku
 */
void peek_two (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 4;
	GtkWidget *window,*vbox, *separator,*back, *hbox;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	hbox = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	board sol, ans;
	initboard(&sol);
	initboard(&ans);
	sol = loadquestion();
	ans = loadsolution();
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(sol.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(sol.boardptr + i * sol.size + j));
				solution[i][j] = gtk_label_new(str);
			}
			else{
				solution[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(solution[i][j]),1);
				char str[2];
				sprintf(str, "%d", *(ans.boardptr + i * sol.size + j));
				gtk_entry_set_text(GTK_ENTRY(solution[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
			}
			gtk_widget_set_size_request(solution[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),solution[i][j],0,0,0);
			
			if((j+1)%2 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%2 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
		
	}
	free(sol.boardptr);
	free(ans.boardptr);
	
	
	/*callback function*/
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 *save_current_progress : saves the current progress of the game
 */
void save_current_progress (GtkWidget *widget, gpointer data){
	board ques,  myans;
	ques = loadquestion();
	int size = myans.size = ques.size;
	myans.boardptr = malloc(size * size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			if(*(ques.boardptr + i * size + j) == 0){
				text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
				*(myans.boardptr + i * size + j) = atoi(text);
			}
			else
				*(myans.boardptr + i * size + j) = *(ques.boardptr + i *size + j);
		}
	}
	savegame(&myans);
}

/*
 *verify_two : submits the board and checks if our solution is correct or not
 */
void verify_two (GtkWidget *widget, gpointer data){
	board ans, myans,ques;
	initboard(&ques);
	initboard(&ans);
	initboard(&myans);
	
	ans = loadsolution();
	ques = loadquestion();
	
	int size = myans.size = 4;
	myans.boardptr = malloc(size * size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			if(*(ques.boardptr + i * size + j) == 0){
				text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
				*(myans.boardptr + i * size + j) = atoi(text);
			}
			else
				*(myans.boardptr + i * size + j) = *(ques.boardptr + i *size + j);
		}
	}	
	if(compareboard(&ans, &myans)){
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("You got it Right!!!!");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "Result");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
			
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
			
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	else{
		GtkWidget *windowno, *labelno, *backno, *vboxno;
		windowno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		labelno = gtk_label_new("Try again!!");
		backno = gtk_button_new_with_label ("BACK");
		vboxno = gtk_vbox_new(1,0);
		
		gtk_window_set_title (GTK_WINDOW (windowno), "Result");
		gtk_window_set_position(GTK_WINDOW(windowno),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowno), 20);
		
		gtk_widget_set_size_request(backno, 40, 50);
		gtk_widget_set_size_request(windowno, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxno), labelno,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxno), backno,0,0,0);
		
		g_signal_connect(backno, "clicked", G_CALLBACK(window_close), windowno);
		
		gtk_container_add(GTK_CONTAINER(windowno), vboxno);
		gtk_widget_show_all(windowno);
		
	}
	free(ans.boardptr);
	free(ques.boardptr);
	free(myans.boardptr);
	
}

/*
 * easy_two : it is a plays menu box for playin 2x2 sudoku easy
 */
void easy_two (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 4;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 4;
	fillValuesEasy(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%2 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%2 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
		
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_two), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_two), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
	
}

/*
 * medium_two : it is a plays menu box for playin 2x2 sudoku medium
 */
void medium_two (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 4;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 4;
	fillValuesMedium(&b);	
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			if((j+1)%2 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%2 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_two), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_two), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 * hard_two : it is a plays menu box for playin 2x2 sudoku hard
 */
void hard_two (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 4;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 4;
	fillValuesHard(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			if((j+1)%2 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%2 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}

	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_two), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_two), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 *play_Sudoku_two : Selection of difficulty level for a 2x2 sudoku
 */
void play_Sudoku_two (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *vbox, *back, *easy, *medium, *hard, *diff;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	diff = gtk_label_new("ARE YOU READY ?");
	back = gtk_button_new_with_label ("BACK");
	easy = gtk_button_new_with_label ("\" NOOB \"");
	medium = gtk_button_new_with_label ("\" PRO \"");
	hard = gtk_button_new_with_label ("\" VETERAN \"");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(easy, 100, 100);
	gtk_widget_set_size_request(medium, 100, 100);
	gtk_widget_set_size_request(hard, 100, 100);	
	gtk_widget_set_size_request(window, 300, 600);
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), diff,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), easy,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), medium,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), hard,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(easy, "clicked", G_CALLBACK(easy_two), window);
	g_signal_connect(medium, "clicked", G_CALLBACK(medium_two), window);
	g_signal_connect(hard, "clicked", G_CALLBACK(hard_two), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * peek_three : allows us to see solutiin for the 3x3 sudoku
 */
void peek_three(GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 9;
	GtkWidget *window,*vbox, *separator,*back, *hbox;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	hbox = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	board sol, ans;
	initboard(&sol);
	initboard(&ans);
	sol = loadquestion();
	ans = loadsolution();
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(sol.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(sol.boardptr + i * sol.size + j));
				solution[i][j] = gtk_label_new(str);
			}
			else{
				solution[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(solution[i][j]),1);
				char str[2];
				sprintf(str, "%d", *(ans.boardptr + i * sol.size + j));
				gtk_entry_set_text(GTK_ENTRY(solution[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
			}
			gtk_widget_set_size_request(solution[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),solution[i][j],0,0,0);
			
			if((j+1)%3 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%3 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
		
	}
	free(sol.boardptr);
	free(ans.boardptr);
	
	
	/*callback function*/
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 *verify_three : submits the board and checks if our solution is correct or not
 */
void verify_three (GtkWidget *widget, gpointer data){
	board ans, myans,ques;
	initboard(&ques);
	initboard(&ans);
	initboard(&myans);
	
	ans = loadsolution();
	ques = loadquestion();
	
	int size = myans.size = 9;
	myans.boardptr = malloc(size * size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			if(*(ques.boardptr + i * size + j) == 0){
				text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
				*(myans.boardptr + i * size + j) = atoi(text);
			}
			else
				*(myans.boardptr + i * size + j) = *(ques.boardptr + i *size + j);
		}
	}	
	if(compareboard(&ans, &myans)){
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("You got it Right!!!!");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "Result");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
			
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
			
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	else{
		GtkWidget *windowno, *labelno, *backno, *vboxno;
		windowno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		labelno = gtk_label_new("Try again!!");
		backno = gtk_button_new_with_label ("BACK");
		vboxno = gtk_vbox_new(1,0);
		
		gtk_window_set_title (GTK_WINDOW (windowno), "Result");
		gtk_window_set_position(GTK_WINDOW(windowno),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowno), 20);
		
		gtk_widget_set_size_request(backno, 40, 50);
		gtk_widget_set_size_request(windowno, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxno), labelno,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxno), backno,0,0,0);
		
		g_signal_connect(backno, "clicked", G_CALLBACK(window_close), windowno);
		
		gtk_container_add(GTK_CONTAINER(windowno), vboxno);
		gtk_widget_show_all(windowno);
		
	}
	free(ans.boardptr);
	free(ques.boardptr);
	free(myans.boardptr);
}

/*
 * easy_three : it is a plays menu box for playin 3x3 sudoku easy
 */
void easy_three (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 9;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 9;
	fillValuesEasy(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%3 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%3 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_three), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_three), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 * medium_three : it is a plays menu box for playin 3x3 sudoku medium
 */
void medium_three (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 9;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 9;
	fillValuesMedium(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%3 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%3 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_three), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_three), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 * hard_three : it is a plays menu box for playin 3x3 sudoku hard
 */
void hard_three (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 9;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 9;
	fillValuesHard(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%3 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%3 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_three), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_three), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 *play_Sudoku_three : Selection of difficulty level for a 3x3 sudoku
 */
void play_Sudoku_three (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *vbox, *back, *easy, *medium, *hard, *diff;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	diff = gtk_label_new("ARE YOU READY ?");
	back = gtk_button_new_with_label ("BACK");
	easy = gtk_button_new_with_label ("\" NOOB \"");
	medium = gtk_button_new_with_label ("\" PRO \"");
	hard = gtk_button_new_with_label ("\" VETERAN \"");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(easy, 100, 100);
	gtk_widget_set_size_request(medium, 100, 100);
	gtk_widget_set_size_request(hard, 100, 100);	
	gtk_widget_set_size_request(window, 300, 600);
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), diff,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), easy,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), medium,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), hard,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(easy, "clicked", G_CALLBACK(easy_three), window);
	g_signal_connect(medium, "clicked", G_CALLBACK(medium_three), window);
	g_signal_connect(hard, "clicked", G_CALLBACK(hard_three), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * peek_three : allows us to see solutiin for the 3x3 sudoku
 */
void peek_four(GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 16;
	GtkWidget *window,*vbox, *separator,*back, *hbox;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	hbox = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	board sol, ans;
	initboard(&sol);
	initboard(&ans);
	sol = loadquestion();
	ans = loadsolution();
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(sol.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(sol.boardptr + i * sol.size + j));
				solution[i][j] = gtk_label_new(str);
			}
			else{
				solution[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(solution[i][j]),1);
				char str[2];
				sprintf(str, "%d", *(ans.boardptr + i * sol.size + j));
				gtk_entry_set_text(GTK_ENTRY(solution[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(solution[i][j]), 0.5);
			}
			gtk_widget_set_size_request(solution[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),solution[i][j],0,0,0);
			
			if((j+1)%4 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%4 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
		
	}
	free(sol.boardptr);
	free(ans.boardptr);
	
	
	/*callback function*/
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 *verify_four : submits the board and checks if our solution is correct or not
 */
void verify_four (GtkWidget *widget, gpointer data){
	board ans, myans,ques;
	initboard(&ques);
	initboard(&ans);
	initboard(&myans);
	
	ans = loadsolution();
	ques = loadquestion();
	
	int size = myans.size = 16;
	myans.boardptr = malloc(size * size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			if(*(ques.boardptr + i * size + j) == 0){
				text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
				*(myans.boardptr + i * size + j) = atoi(text);
			}
			else
				*(myans.boardptr + i * size + j) = *(ques.boardptr + i *size + j);
		}
	}	
	if(compareboard(&ans, &myans)){
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("You got it Right!!!!");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "Result");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
			
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
			
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	else{
		GtkWidget *windowno, *labelno, *backno, *vboxno;
		windowno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		labelno = gtk_label_new("Try again!!");
		backno = gtk_button_new_with_label ("BACK");
		vboxno = gtk_vbox_new(1,0);
		
		gtk_window_set_title (GTK_WINDOW (windowno), "Result");
		gtk_window_set_position(GTK_WINDOW(windowno),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowno), 20);
		
		gtk_widget_set_size_request(backno, 40, 50);
		gtk_widget_set_size_request(windowno, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxno), labelno,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxno), backno,0,0,0);
		
		g_signal_connect(backno, "clicked", G_CALLBACK(window_close), windowno);
		
		gtk_container_add(GTK_CONTAINER(windowno), vboxno);
		gtk_widget_show_all(windowno);
		
	}
	free(ans.boardptr);
	free(ques.boardptr);
	free(myans.boardptr);
}

/*
 * easy_four : it is a plays menu box for playin 4x4 sudoku hard
 */
void easy_four (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 16;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 16;
	fillValuesEasy(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),2);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%4 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%4 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_four), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 * medium_four : it is a plays menu box for playin 4x4 sudoku hard
 */
void medium_four (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 16;
	GtkWidget *window, *hbox, *vbox, *separator, *submit, *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 16;
	fillValuesMedium(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),2);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%4 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%4 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_four), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 * hard_four : it is a plays menu box for playin 4x4 sudoku hard
 */
void hard_four (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 16;
	GtkWidget *window, *hbox, *vbox, *separator, *submit,   *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	board b;
	initboard(&b);
	b.size = 16;
	fillValuesEasy(&b);
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(b.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),2);
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%4 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%4 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	/*callback function*/
	g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_four), window);
	g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
	g_signal_connect(submit, "clicked", G_CALLBACK(verify_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window);
}

/*
 *play_Sudoku_three : Selection of difficulty level for a 4x4 sudoku
 */
void play_Sudoku_four (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *vbox, *back, *easy, *medium, *hard, *diff;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	diff = gtk_label_new("ARE YOU READY ?");
	back = gtk_button_new_with_label ("BACK");
	easy = gtk_button_new_with_label ("\" NOOB \"");
	medium = gtk_button_new_with_label ("\" PRO \"");
	hard = gtk_button_new_with_label ("\" VETERAN \"");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(easy, 100, 100);
	gtk_widget_set_size_request(medium, 100, 100);
	gtk_widget_set_size_request(hard, 100, 100);	
	gtk_widget_set_size_request(window, 300, 600);
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), diff,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), easy,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), medium,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), hard,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(easy, "clicked", G_CALLBACK(easy_four), window);
	g_signal_connect(medium, "clicked", G_CALLBACK(medium_four), window);
	g_signal_connect(hard, "clicked", G_CALLBACK(hard_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * play_Sudoku : selection of size of board
 */
void play_Sudoku (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *vbox, *back, *two, *three, *four, *sizeofboard;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	sizeofboard = gtk_label_new("Select Size");
	back = gtk_button_new_with_label ("BACK");
	two = gtk_button_new_with_label ("2X2");
	three = gtk_button_new_with_label ("3X3");
	four = gtk_button_new_with_label ("4X4");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "SOLVE GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(two, 100, 100);
	gtk_widget_set_size_request(three, 100, 100);
	gtk_widget_set_size_request(four, 100, 100);
	gtk_widget_set_size_request(window, 300, 700);
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), sizeofboard,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), two,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), three,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), four,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(two, "clicked", G_CALLBACK(play_Sudoku_two), window);
	g_signal_connect(three, "clicked", G_CALLBACK(play_Sudoku_three), window);
	g_signal_connect(four, "clicked", G_CALLBACK(play_Sudoku_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	
}

/*
 * play_game : rules for playing Sudoku
 */
void play_game (GtkWidget *widget, gpointer data){
	gtk_widget_destroy(GTK_WIDGET(data));
	GtkWidget *window, *RULES, *play_button, *back, *vbox;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	RULES = gtk_label_new("\n\t\t\t\t\t\t\t\t\tRULES\n\n\t\t\t\t\t\tHOW TO ENTER ELEMENTS\n\n\n1.Just click the number that you have to enter\n\n2.Each row and column start from 0 and end at size you have entered \nenter your row and coloum\n\n3.Enter Row then Colum and then the element that is to be filled.\n\n4.If an element entered in a wrong location has to be deleted,\nenter zero in that location\n");
	play_button = gtk_button_new_with_label ("PLAY GAME");
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	
	/*size of widgets*/
	gtk_widget_set_size_request(play_button, 50, 50);
	gtk_widget_set_size_request(back, 50, 50);
	gtk_widget_set_size_request(vbox, 500, 500);
	
	/*arranging widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), RULES,0,0,60);
	gtk_box_pack_start(GTK_BOX(vbox), play_button,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(play_button, "clicked", G_CALLBACK(play_Sudoku), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	
}

/*
 * lost : ask permission to overwrites our previous progress
 */
void lost(GtkWidget *widget, gpointer data){
	GtkWidget *window, *OK, *back, *label,*vbox,*hbox;
	vbox = gtk_vbox_new(0,0);
	hbox = gtk_hbox_new(0,0);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label = gtk_label_new("All your previous progress will be lost\nDo you still wish to continue?");
	OK = gtk_button_new_with_label ("OK");
	back = gtk_button_new_with_label ("BACK");
	
	gtk_window_set_title (GTK_WINDOW (window), "------");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	gtk_widget_set_size_request(back, 80, 50);
	gtk_widget_set_size_request(OK, 80, 50);
	gtk_widget_set_size_request(window, 450, 200);
		
	gtk_box_pack_start(GTK_BOX(vbox), label,0,0,30);
	gtk_box_pack_start(GTK_BOX(hbox), back,0,0,80);
	gtk_box_pack_start(GTK_BOX(hbox), OK,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox,0,0,0);
	
		
	g_signal_connect(OK, "clicked", G_CALLBACK(play_game), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	
}

/*
 *solvegameclickedtwo: solves 2x2 Sudoku
 */
void solvegameclickedtwo(GtkWidget *widget, gpointer data){
	board b;
	initboard(&b);
	int size = b.size = 4;
	b.boardptr = malloc(b.size * b.size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
			*(b.boardptr + i * b.size + j) = atoi(text);
		}
	}
	if(fillSudoku(&b, 0, 0)){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++ ){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				gtk_entry_set_text(GTK_ENTRY(no[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			}
		}
	}
	else{
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("Oops!! Try again....");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "ERROR");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	free(b.boardptr);
	
}
/*
 * solve_Sudoku_two : dialouge box for solving Sudoku of size 2x2
 */
void solve_Sudoku_two(GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 4;
	GtkWidget *window, *hbox, *vbox, *separator, *solvegame,   *back;
	
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	solvegame = gtk_button_new_with_label ("SOLVE GAME");
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "2X2");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			no[i][j] = gtk_entry_new();
			gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			if((j+1)%2 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,1);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%2 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,1);
			}

	}
	
	/*callback function*/
	g_signal_connect(solvegame, "clicked", G_CALLBACK(solvegameclickedtwo), NULL);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),solvegame,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 *solvegameclickedtwo: solves 3x3 Sudoku
 */
void solvegameclickedthree(GtkWidget *widget, gpointer data){
	board b;
	initboard(&b);
	int size = b.size = 9;
	b.boardptr = malloc(b.size * b.size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
			*(b.boardptr + i * b.size + j) = atoi(text);
		}
	}
	if(fillSudoku(&b, 0, 0)){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++ ){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				gtk_entry_set_text(GTK_ENTRY(no[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			}
		}
	}
	else{
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("Oops!! Try again....");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "ERROR");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	free(b.boardptr);
}

/*
 * solve_Sudoku_three : dialouge box for solving Sudoku of size 3x3
 */
void solve_Sudoku_three(GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 9;
	GtkWidget *window, *hbox, *vbox, *separator, *solvegame,   *back;
	
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	solvegame = gtk_button_new_with_label ("SOLVE GAME");
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "3X3");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			no[i][j] = gtk_entry_new();
			gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),1);
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			if((j+1)%3 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,1);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%3 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,1);
			}
	}
	
	/*callback function*/
	g_signal_connect(solvegame, "clicked", G_CALLBACK(solvegameclickedthree), NULL);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),solvegame,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 *solvegameclickedtwo: solves 3x3 Sudoku
 */
void solvegameclickedfour(GtkWidget *widget, gpointer data){
	board b;
	initboard(&b);
	int size = b.size = 16;
	b.boardptr = malloc(b.size * b.size * sizeof(int));
	const gchar *text;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++ ){
			text = gtk_entry_get_text(GTK_ENTRY(no[i][j]));
			*(b.boardptr + i * b.size + j) = atoi(text);
		}
	}
	if(fillSudoku(&b, 0, 0)){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++ ){
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				gtk_entry_set_text(GTK_ENTRY(no[i][j]),str);
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			}
		}
	}
	else{
		GtkWidget *windowels, *OK, *backels, *label,*vboxels,*hboxels;
		vboxels = gtk_vbox_new(0,0);
		hboxels = gtk_hbox_new(0,0);
		windowels = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		label = gtk_label_new("Oops!! Try again....");
		OK = gtk_button_new_with_label ("OK");
		backels = gtk_button_new_with_label ("BACK");
		
		gtk_window_set_title (GTK_WINDOW (windowels), "ERROR");
		gtk_window_set_position(GTK_WINDOW(windowels),GTK_WIN_POS_CENTER);
		gtk_container_set_border_width (GTK_CONTAINER (windowels), 20);
		
		gtk_widget_set_size_request(backels, 80, 50);
		gtk_widget_set_size_request(OK, 80, 50);
		gtk_widget_set_size_request(windowels, 450, 200);
		
		gtk_box_pack_start(GTK_BOX(vboxels), label,0,0,50);
		gtk_box_pack_start(GTK_BOX(hboxels), backels,0,0,80);
		gtk_box_pack_start(GTK_BOX(hboxels), OK,0,0,0);
		gtk_box_pack_start(GTK_BOX(vboxels), hboxels,0,0,0);
		
		g_signal_connect(OK, "clicked", G_CALLBACK(window_close), windowels);
		g_signal_connect(backels, "clicked", G_CALLBACK(window_close), windowels);
		
		gtk_container_add(GTK_CONTAINER(windowels), vboxels);
		gtk_widget_show_all(windowels);
	}
	free(b.boardptr);
}

/*
 * solve_Sudoku_four : dialouge box for solving Sudoku of size 4x4
 */
void solve_Sudoku_four(GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	int size = 16;
	GtkWidget *window, *hbox, *vbox, *separator, *solvegame,   *back;
	
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	solvegame = gtk_button_new_with_label ("SOLVE GAME");
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "4X4");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*code*/
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			no[i][j] = gtk_entry_new();
			gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
			gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),2);
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			if((j+1)%4 == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,1);
			}
			
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%4 == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,1);
			}
	}
	
	/*callback function*/
	g_signal_connect(solvegame, "clicked", G_CALLBACK(solvegameclickedfour), NULL);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_box_pack_start(GTK_BOX(vbox),solvegame,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox),back,0,0,0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * solve_Sudoku : selection of size of board question that is to be solved
 */
void solve_Sudoku (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *vbox, *back, *two, *three, *four, *sizeofboard;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	sizeofboard = gtk_label_new("Select Size");
	back = gtk_button_new_with_label ("BACK");
	two = gtk_button_new_with_label ("2X2");
	three = gtk_button_new_with_label ("3X3");
	four = gtk_button_new_with_label ("4X4");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "SOLVE GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(two, 100, 100);
	gtk_widget_set_size_request(three, 100, 100);
	gtk_widget_set_size_request(four, 100, 100);
	gtk_widget_set_size_request(window, 300, 700);
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), sizeofboard,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), two,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), three,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), four,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(two, "clicked", G_CALLBACK(solve_Sudoku_two), window);
	g_signal_connect(three, "clicked", G_CALLBACK(solve_Sudoku_three), window);
	g_signal_connect(four, "clicked", G_CALLBACK(solve_Sudoku_four), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * solve_game : ruels of Solving a suoku puzzel
 */
void solve_game (GtkWidget *widget, gpointer data){
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *RULES, *solve_button, *back, *vbox;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	RULES = gtk_label_new("\n\t\t\t\t\t\t\tRULES\n\n\t\t\t\tHOW TO ENTER ELEMENTS\n\n\n1. Enter the size of the board that you desire to get solved.\n\n2. Enter the elements int their appropriate loaction.\n\n3. Enter '0' for the elments that are to be filled.\n\n");
	solve_button = gtk_button_new_with_label ("SOLVE GAME");
	back = gtk_button_new_with_label ("BACK");
	vbox = gtk_vbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "SOLVE GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	
	/*size of widgets*/
	gtk_widget_set_size_request(solve_button, 50, 50);
	gtk_widget_set_size_request(back, 50, 50);
	gtk_widget_set_size_request(vbox, 500, 500);
	
	/*arranging widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), RULES,0,0,70);
	gtk_box_pack_start(GTK_BOX(vbox), solve_button,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), back,0,0,0);
	
	/*callback function*/
	g_signal_connect(solve_button, "clicked", G_CALLBACK(solve_Sudoku), window);
	g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
	g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

/*
 * loads our previous progress
 */
void load_game (GtkWidget *widget, gpointer data){
	board b,ques;
	initboard(&b);
	initboard(&ques);
	b = loadgame();
	ques = loadquestion();
	int size = ques.size;
	gtk_widget_hide(GTK_WIDGET(data));
	GtkWidget *window, *hbox, *vbox, *separator, *submit,   *back, *view_solution, *save, *hboxm, *vboxsidemenua, *vboxsidemenub;
	
	/*initializing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	submit = gtk_button_new_with_label ("SUBMIT");
	view_solution = gtk_button_new_with_label ("      PEEK\nSOLUTION");
	back = gtk_button_new_with_label ("BACK");
	save = gtk_button_new_with_label ("SAVE");
	vbox = gtk_vbox_new(0,0);
	vboxsidemenua = gtk_vbox_new(0,0);
	vboxsidemenub = gtk_vbox_new(0,0);
	hboxm = gtk_hbox_new(0,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "PLAY GAME");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(submit, 100, 100);
	gtk_widget_set_size_request(view_solution, 100, 100);
	gtk_widget_set_size_request(back, 100, 100);
	gtk_widget_set_size_request(save, 100, 100);
	
	/*code*/
	for(int i = 0; i < size; i++){
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < size; j++){
			if(*(ques.boardptr + i * size + j) != 0){
				char str[2];
				sprintf(str, "%d", *(ques.boardptr + i * b.size + j));
				no[i][j] = gtk_label_new(str);
			}
			else{
				no[i][j] = gtk_entry_new();
				gtk_entry_set_alignment(GTK_ENTRY(no[i][j]), 0.5);
				gtk_entry_set_max_length(GTK_ENTRY(no[i][j]),2);
				char str[2];
				sprintf(str, "%d", *(b.boardptr + i * b.size + j));
				
				gtk_entry_set_text(GTK_ENTRY(no[i][j]),str);
				
				
			}
			gtk_widget_set_size_request(no[i][j], 50, 50);
			gtk_box_pack_start(GTK_BOX(hbox),no[i][j],0,0,0);
			
			if((j+1)%squareroot(size) == 0){
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,2);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%squareroot(size) == 0){
				separator = gtk_hseparator_new();
				gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,2);
			}
	}
	
	/*arranging the widgets*/
	gtk_box_pack_start(GTK_BOX(hboxm),vbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),save,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenua),view_solution,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),submit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vboxsidemenub),back,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenua,0,0,0);
	gtk_box_pack_start(GTK_BOX(hboxm),vboxsidemenub,0,0,0);
	
	if(size == 4){
	/*callback function*/
		g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_two), window);
		g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
		g_signal_connect(submit, "clicked", G_CALLBACK(verify_two), window);
		g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
		g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	}
	
	if(size == 9){
	/*callback function*/
		g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_three), window);
		g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
		g_signal_connect(submit, "clicked", G_CALLBACK(verify_three), window);
		g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
		g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	}
	
	if(size == 16){
	/*callback function*/
		g_signal_connect(view_solution, "clicked", G_CALLBACK(peek_four), window);
		g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
		g_signal_connect(submit, "clicked", G_CALLBACK(verify_four), window);
		g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
		g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	}
	
	if(size == 25){
	/*callback function*/
		g_signal_connect(save, "clicked", G_CALLBACK(save_current_progress), window);
		g_signal_connect(back, "clicked", G_CALLBACK(window_show), GTK_WIDGET(data));
		g_signal_connect(back, "clicked", G_CALLBACK(window_close), window);
	}
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), hboxm);
	gtk_widget_show_all(window); 
	
}

int main(int argc, char* argv[]){
	gtk_init(&argc, &argv);
	GtkWidget *window, *play, *solve, *load, *exit, *WEL, *vbox;
	
	/*initalizing*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	play = gtk_button_new_with_label ("PLAY GAME");
	solve = gtk_button_new_with_label ("SOLVE GAME");
	load = gtk_button_new_with_label ("LOAD GAME");
	exit = gtk_button_new_with_label ("EXIT GAME");
	WEL = gtk_label_new("WELCOME TO SUDOKU GAME");
	vbox = gtk_vbox_new(1,0);
	
	/*window*/
	gtk_window_set_title (GTK_WINDOW (window), "Sudoku");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	
	/*size of widgets*/
	gtk_widget_set_size_request(WEL, 100, 100);
	gtk_widget_set_size_request(play, 100, 100);
	gtk_widget_set_size_request(solve, 100, 100);
	gtk_widget_set_size_request(load, 100, 100);
	gtk_widget_set_size_request(exit, 100, 100);	
	gtk_widget_set_size_request(window, 300, 600);
	
	/*arranging widgets*/
	gtk_box_pack_start(GTK_BOX(vbox), WEL,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), play,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), solve,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), load,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox), exit,0,0,0);
	
	/*callback functions*/
	g_signal_connect(play, "clicked", G_CALLBACK(lost), window);
	g_signal_connect(solve, "clicked", G_CALLBACK(solve_game), window);
	g_signal_connect(load, "clicked", G_CALLBACK(load_game), window);
	g_signal_connect(exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	
	/*ending*/
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
