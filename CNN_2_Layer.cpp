#include <iostream>
#include <random>
#include <stdlib.h>

#define max(a, b) (a>b)?a:b

typedef unsigned long ulong;

using namespace std;

int memory_alloc_2D(ulong ***arr, int height, int width) {
	*arr = (ulong**)calloc(height, sizeof(ulong*));

	for (int i = 0; i < height; i++) {
		(*arr)[i] = (ulong*)calloc(width, sizeof(ulong));
	}
	return 0;
}


ulong** Convolution (ulong **arr, int height, int width, ulong **filter,int filter_width) {
	int out_height = (height - filter_width + 1);
	int out_width =	(width - filter_width + 1);


	ulong** conv_out;
	memory_alloc_2D(&conv_out, out_height, out_width);

	
	for (int i = 0; i < out_height; i += 1) {
		for (int j = 0; j < out_width; j += 1) {
			//filter output
			ulong ret = 0;
			for (int ii = 0; ii < filter_width; ii++) {
				for (int jj = 0; jj < filter_width; jj++) {
					ret += arr[i + ii][j + jj] * filter[ii][jj];
				}
			}
			conv_out[i][j] = ret;
		}
	}

	return conv_out;
}

ulong** relu (ulong **arr, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			arr[i][j] = max(arr[i][j], 0);
		}
	}
	return arr;
}
//max pooling
ulong** pooling (ulong **arr, int pool_width, int height, int width) {
	for (int i = 0; i < (height - pool_width); i += pool_width) {
		for (int j = 0; j < (width - pool_width); j += pool_width) {
			int max_num = arr[i][j];
			//calculate max
			for (int ii = 0; ii < pool_width; ii++) {
				for (int jj = 0; jj < pool_width; jj++) {
					max_num = max(arr[i + ii][j + jj], max_num);
				}
			}
		}
	}
}

int main() {
	int height = 10000; 
	int width = 10000;
	int filter_width = 5;
	int pool_width = 2;

	ulong **input;
	memory_alloc_2D(&input, height, width);

	// std::default_random_engine generator;
	// std::uniform_real_distribution<ulong> distribution(0, 255);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			input[i][j] = i*width + j;
		}
	}

	ulong **filter;
	memory_alloc_2D(&filter, filter_width, filter_width);

	for (int i = 0; i < filter_width; i++) {
		for (int j = 0; j < filter_width; j++) {
			filter[i][j] = filter_width * i + j;
		}
	}

	ulong **conv_out;
	conv_out =	Convolution(input, height, width, filter, filter_width);
	int out_height = pool_width*((height - filter_width + 1)/pool_width);
	int out_width = pool_width*((width - filter_width + 1)/pool_width);

	ulong **relu_out = relu(conv_out, out_height, out_width);

	ulong **pooling_out = pooling(relu_out, pool_width, out_height, out_width);

	int pool_out_height = out_height/pool_width;
	int pool_out_width = out_width/pool_width;
}