// SDLAW, a port/conversion of the DOS game Another World.
// MediaExplorer 2023

// SEG000.C
void far pop_main(); 
void load_sound_names();
void __pascal far draw_full_image(enum full_image_id id);

// SEG009.C
void sdlperror(const char* header);
bool file_exists(const char* filename);
#define locate_file(filename) locate_file_(filename, (char*)alloca(POP_MAX_PATH), POP_MAX_PATH)
const char* locate_file_(const char* filename, char* path_buffer, int buffer_size);

#ifdef _WIN32
FILE* fopen_UTF8(const char* filename, const char* mode);
#define fopen fopen_UTF8
int chdir_UTF8(const char* path);
#define chdir chdir_UTF8
int mkdir_UTF8(const char* path);
#define mkdir mkdir_UTF8
int access_UTF8(const char* filename_UTF8, int mode);
#ifdef access
#undef access
#endif
#define access access_UTF8
#endif //_WIN32

directory_listing_type* create_directory_listing_and_find_first_file(
    const char* directory, const char* extension);

char* get_current_filename_from_directory_listing(directory_listing_type* data);
bool find_next_file(directory_listing_type* data);
void close_directory_listing(directory_listing_type* data);
int __pascal far read_key();
void __pascal far clear_kbd_buf();
word __pascal far prandom(word max);
int __pascal far round_xpos_to_byte(int xpos,int round_direction);
void __pascal far show_dialog(const char *text);
void __pascal far quit(int exit_code);
void __pascal far restore_stuff();
int __pascal far key_test_quit();
const char* __pascal far check_param(const char *param);
int __pascal far pop_wait(int timer_index,int time);
dat_type *__pascal open_dat(const char *file,int drive);
void __pascal far set_loaded_palette(dat_pal_type far *palette_ptr);
chtab_type* __pascal load_sprites_from_file(int resource,int palette_bits, int quit_on_error);
void __pascal far free_chtab(chtab_type *chtab_ptr);
image_type* decode_image(image_data_type* image_data, dat_pal_type* palette);
image_type*far __pascal far load_image(int index, dat_pal_type* palette);
void __pascal far draw_image_transp(image_type far *image,image_type far *mask,int xpos,int ypos);
int __pascal far set_joy_mode();
surface_type far *__pascal make_offscreen_buffer(const rect_type far *rect);
void __pascal far free_surface(surface_type *surface);
void __pascal far free_peel(peel_type *peel_ptr);
void __pascal far set_hc_pal();
void __pascal far flip_not_ega(byte far *memory,int height,int stride);
void __pascal far flip_screen(surface_type far *surface);
void __pascal far fade_in_2(surface_type near *source_surface,int which_rows);
void __pascal far fade_out_2(int rows);
void __pascal far draw_image_transp_vga(image_type far *image,int xpos,int ypos);
int __pascal far get_line_width(const char far *text,int length);
int __pascal far draw_text_character(byte character);
void __pascal far draw_rect(const rect_type far *rect,int color);
void draw_rect_with_alpha(const rect_type* rect, byte color, byte alpha);
void draw_rect_contours(const rect_type* rect, byte color);
surface_type far *__pascal rect_sthg(surface_type *surface,const rect_type far *rect);
rect_type far *__pascal shrink2_rect(
    rect_type far *target_rect,const rect_type far *source_rect,int delta_x,int delta_y);
void __pascal far set_curr_pos(int xpos,int ypos);
void __pascal far restore_peel(peel_type *peel_ptr);
peel_type* __pascal far read_peel_from_screen(const rect_type far *rect);
void __pascal far show_text(
    const rect_type far *rect_ptr,int x_align,int y_align,const char far *text);
int __pascal far intersect_rect(
    rect_type far *output,const rect_type far *input1,const rect_type far *input2);
rect_type far * __pascal far union_rect(
    rect_type far *output,const rect_type far *input1,const rect_type far *input2);
void __pascal far stop_sounds();
void init_digi();
void __pascal far play_sound_from_buffer(sound_buffer_type far *buffer);
void turn_music_on_off(byte new_state);
void __pascal far turn_sound_on_off(byte new_state);
int __pascal far check_sound_playing();
void apply_aspect_ratio();
void window_resized();
void __pascal far set_gr_mode(byte grmode);

SDL_Surface* get_final_surface();

void update_screen();

void __pascal far set_pal_arr(int start,int count,const rgb_type far *array,int vsync);
void __pascal far set_pal(int index,int red,int green,int blue,int vsync);
int __pascal far add_palette_bits(byte n_colors);

int __pascal far find_first_pal_row(int which_rows_mask);
int __pascal far get_text_color(int cga_color,int low_half,int high_half_mask);
void __pascal far close_dat(dat_type far *pointer);
void far *__pascal load_from_opendats_alloc(int resource, const char* extension, data_location* out_result, int* out_size);
int __pascal far load_from_opendats_to_area(int resource,void far *area,int length, const char* extension);
void rect_to_sdlrect(const rect_type* rect, SDL_Rect* sdlrect);
void __pascal far method_1_blit_rect(
    surface_type near *target_surface,surface_type near *source_surface,const rect_type far *target_rect, const rect_type far *source_rect,int blit);
image_type far * __pascal far method_3_blit_mono(
    image_type far *image,int xpos,int ypos,int blitter,byte color);
const rect_type far * __pascal far method_5_rect(
    const rect_type far *rect,int blit,byte color);
void draw_rect_with_alpha(const rect_type* rect, byte color, byte alpha);
image_type far * __pascal far method_6_blit_img_to_scr(
    image_type far *image,int xpos,int ypos,int blit);
void reset_timer(int timer_index);
double get_ticks_per_sec(int timer_index);
void set_timer_length(int timer_index, int length);
void __pascal start_timer(int timer_index, int length);
int __pascal do_wait(int timer_index);
void __pascal far init_timer(int frequency);
void __pascal far set_clip_rect(const rect_type far *rect);
void __pascal far reset_clip_rect();
void __pascal far set_bg_attr(int vga_pal_index,int hc_pal_index);
rect_type far *__pascal offset4_rect_add
(rect_type far *dest, const rect_type far *source,int d_left,int d_top,int d_right,int d_bottom);
int __pascal far input_str(const rect_type far *rect,char *buffer,int max_length,const char *initial,int has_initial,int arg_4,int color,int bgcolor);
rect_type far *__pascal offset2_rect(
    rect_type far *dest, const rect_type far *source,int delta_x,int delta_y);
void __pascal far show_text_with_color(
    const rect_type far *rect_ptr,int x_align,int y_align, const char far *text,int color);
void __pascal do_simple_wait(int timer_index);
void process_events();
void idle();
void __pascal far init_copyprot_dialog();
dialog_type * __pascal far make_dialog_info(dialog_settings_type *settings, rect_type *dialog_rect,
                                            rect_type *text_rect, peel_type *dialog_peel);
void __pascal far calc_dialog_peel_rect(dialog_type*dialog);
void __pascal far read_dialog_peel(dialog_type *dialog);
void __pascal far draw_dialog_frame(dialog_type *dialog);
void __pascal far add_dialog_rect(dialog_type *dialog);
void __pascal far dialog_method_2_frame(dialog_type *dialog);
#ifdef USE_FADE
void __pascal far fade_in_2(surface_type near *source_surface,int which_rows);
palette_fade_type far *__pascal make_pal_buffer_fadein(
    surface_type *source_surface,int which_rows,int wait_time);
void __pascal far pal_restore_free_fadein(palette_fade_type far *palette_buffer);
int __pascal far fade_in_frame(palette_fade_type far *palette_buffer);
void __pascal far fade_out_2(int rows);
palette_fade_type far *__pascal make_pal_buffer_fadeout(int which_rows,int wait_time);
void __pascal far pal_restore_free_fadeout(palette_fade_type far *palette_buffer);
int __pascal far fade_out_frame(palette_fade_type far *palette_buffer);
void __pascal far read_palette_256(rgb_type far *target);
void __pascal far set_pal_256(rgb_type far *source);
#endif
void set_chtab_palette(chtab_type* chtab, byte* colors, int n_colors);
int has_timer_stopped(int index);
sound_buffer_type* load_sound(int index);
void free_sound(sound_buffer_type far *buffer);

// SEQTABLE.C
void apply_seqtbl_patches();
#ifdef CHECK_SEQTABLE_MATCHES_ORIGINAL
void check_seqtable_matches_original();
#endif

// OPTIONS.C
void turn_fixes_and_enhancements_on_off(byte new_state);
void turn_custom_options_on_off(byte new_state);
void set_options_to_default();
void load_global_options();
void check_mod_param();
void load_mod_options();
int process_rw_write(SDL_RWops* rw, void* data, size_t data_size);
int process_rw_read(SDL_RWops* rw, void* data, size_t data_size);
void load_dos_exe_modifications(const char* folder_name);


// midi.c
void stop_midi();
void init_midi();
void midi_callback(void *userdata, Uint8 *stream, int len);
void __pascal far play_midi_sound(sound_buffer_type far *buffer);
