#include "commons.h"
#include "seconds.h"
	
const uint8_t NUMBER_OF_BOXES = 11;
const uint8_t BOX_PADDING = 3;
const uint8_t BOX_SPACE = 1;
const uint8_t BOX_SIZE = 14;
Layer box_layer; //Box to be animated
PropertyAnimation box_anim; 
const int AnimationTime = 150;
	
void display_box(struct Layer *l, GContext *ctx) { //box_layer constantly shows a box
	graphics_context_set_fill_color(ctx, FRONT_COLOR);
	graphics_fill_rect(ctx, GRect(0,0,BOX_SIZE,BOX_SIZE), 0, GCornerNone);
	;
}

void anim_box(int8_t box_position) { 
	GRect from_frame = layer_get_frame(&box_layer); 
	GRect to_frame = from_frame;
	//adjusting original and upcoming layer frame
	if(box_position == 0) { //box will slide down from above
		from_frame.origin.y = -BOX_SIZE;
	}
	if(box_position == -1 ) { //for seconds 0-4 hides box under the screen
		to_frame.origin.y = 169;
	}
	else { // for seconds 5-59 counts where to move the box
		to_frame.origin.y = BOX_PADDING + box_position*(BOX_SIZE+BOX_SPACE);
	}
	
	
	property_animation_init_layer_frame(&box_anim, &box_layer, &from_frame, &to_frame);
	animation_set_curve(&box_anim.animation, AnimationCurveEaseInOut );
	animation_set_duration(&box_anim.animation, AnimationTime);
	animation_schedule(&box_anim.animation);
}

void count_seconds(unsigned short seconds_passed) {
	if (seconds_passed%5 == 0) { //animate every 5 seconds
		anim_box(seconds_passed/5 - 1);
	}
}

void seconds_first_display(unsigned short seconds_passed) {
	int8_t box_position = seconds_passed/5 - 1;
	GRect box_frame = GRect(144-(BOX_PADDING + BOX_SIZE), -BOX_SIZE, BOX_SIZE, BOX_SIZE); //invisible
	if(box_position > -1) {box_frame.origin.y = BOX_PADDING + box_position*(BOX_SPACE+BOX_SIZE);} //to relocate box if should be visible (seconds 5-59)
	layer_init(&box_layer, box_frame);
	layer_set_update_proc(&box_layer, &display_box);
	layer_add_child(window_get_root_layer(&window), &box_layer);
}
