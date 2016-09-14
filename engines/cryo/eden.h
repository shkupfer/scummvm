/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef CRYO_EDEN_H
#define CRYO_EDEN_H

#include "cryo/defs.h"

#define EDEN_DEBUG

namespace Cryo {

class EdenGame {
public:
	void run();

private:
	void RemoveConsole();
	void scroll();
	void resetscroll();
	void scrollfresques();
	void afffresques();
	void gametofresques();
	void dofresques();
	void finfresques();
	void scrollmiroir();
	void scrollpano();
	void affsuiveur(suiveur_t *suiveur, short x, short y);
	void persoinmiroir();
	void gametomiroir(unsigned char arg1);
	void flipmode();
	void quitmiroir();
	void clictimbre();
	void clicplanval();
	void gotolieu(goto_t *go);
	void deplaval(unsigned short roomNum);
	void deplacement(short dir);
	void deplacement2(short dir);
	void dinosoufle();
	void plaquemonk();
	void fresquesgraa();
	void fresqueslasc();
	void pushpierre();
	void tetemomie();
	void tetesquel();
	void squelmoorkong();
	void choisir();
	void dinaparle();
	void roiparle();
	void roiparle1();
	void roiparle2();
	void roiparle3();
	void getcouteau();
	void getprisme();
	void getchampb();
	void getchampm();
	void getor();
	void getnido();
	void getnidv();
	void getcorne();
	void getsoleil();
	void getoeuf();
	void getplaque();
	void voirlac();
	void gotohall();
	void demitourlabi();
	void gotonido();
	void gotoval();
	void visiter();
	void final();
	void goto_nord();
	void goto_est();
	void goto_sud();
	void goto_ouest();
	void afficher();
	void afficher128();
	void sauvefrises();
	void sauvefriseshaut(short x);
	void sauvefrisesbas();
	void restaurefrises();
	void restaurefriseshaut();
	void restaurefrisesbas();
	void use_main_bank();
	void use_bank(short bank);
	void sundcurs(short x, short y);
	void rundcurs();
	void noclipax(short index, short x, short y);
	void noclipax_avecnoir(short index, short x, short y);
	void getglow(short x, short y, short w, short h);
	void unglow();
	void glow(short index);
	void readpalette(unsigned char *ptr);
	void spritesurbulle(short index, short x, short y);
	void bars_out();
	void bars_in();
	void sauvefondbouche();
	void restaurefondbouche();
	void blackbars();
	void afftopscr();
	void affplanval();
	void affrepere(short index, short location);
	void affrepereadam(short location);
	void rest_repadam();
	void save_repadam(short x, short y);
	char istrice(short roomNum);
	char istyran(short roomNum);
	void istyranval(area_t *area);
	char getdirection(perso_t *perso);
	char caselibre(char loc, perso_t *perso);
	void melange1(char elem[4]);
	void melange2(char elem[4]);
	void melangedir();
	char naitredino(char persoType);
	void newcita(char arg1, short arg2, room_t *room);
	void citaevol(short level);
	void citacapoute(short roomNum);
	void buildcita();
	void citatombe(char level);
	void constcita();
	void depladino(perso_t *perso);
	void deplaalldino();
	void newvallee();
	char whereiscita();
	char iscita(short loc);
	void lieuvava(area_t *area);
	void vivredino();
	void vivreval(short areaNum);
	void chaquejour();
	void temps_passe(short t);
	void heurepasse();
	void anim_perso();
	void getanimrnd();
	void addanim();
	void virespritebouche();
	void anim_perfin();
	void perso_spr(unsigned char *spr);
	void af_image();
	void af_perso1();
	void af_perso();
	void ef_perso();
	void load_perso(perso_t *perso);
	void load_perso_cour();
	void fin_perso();
	void no_perso();
	void close_perso();
	void af_fondsuiveur();
	void af_fondperso1();
	void af_fondperso();
	void setpersoicon();
	void show_perso();
	void showpersopanel();
	void getdatasync();
	short ReadNombreFrames();
	void waitendspeak();
	void my_bulle();
	void my_pr_bulle();
	void charsurbulle(unsigned char c, unsigned char color, short width);
	void af_subtitle();
	void sauvefondbulle(short y);
	void restaurefondbulle();
	void af_subtitlehnm();
	void patchphrase();
	void vavapers();
	void citadelle();
	void choixzone();
	void showevents1();
	void showevents();
	void parle_mfin();
	void parlemoi_normal();
	void parle_moi();
	void init_perso_ptr(perso_t *perso);
	void perso1(perso_t *perso);
	void perso_normal(perso_t *perso);
	void persoparle(short pers);
	void roi();
	void dina();
	void thoo();
	void monk();
	void bourreau();
	void messager();
	void mango();
	void eve();
	void azia();
	void mammi();
	void gardes();
	void bambou();
	void kabuka();
	void fisher();
	void dino();
	void tyran();
	void morkus();
	void comment();
	void adam();
	void init_oui();
	void init_non();
	void oui();
	void non();
	char verif_oui();
	void SpcChampi(perso_t *perso);
	void SpcNidv(perso_t *perso);
	void SpcNido(perso_t *perso);
	void SpcPomme(perso_t *perso);
	void SpcOr(perso_t *perso);
	void SpcPrisme(perso_t *perso);
	void SpcTalisman(perso_t *perso);
	void SpcMasque(perso_t *perso);
	void SpcSac(perso_t *perso);
	void SpcTrompet(perso_t *perso);
	void SpcArmes(perso_t *perso);
	void SpcInstru(perso_t *perso);
	void SpcOeuf(perso_t *perso);
	void TyranMeurt(perso_t *perso);
	void SpecialObjets(perso_t *perso, char objid);
	void dialautoon();
	void dialautooff();
	void follow();
	void dialonfollow();
	void abortdial();
	void narrateur();
	void vrf_phrases_file();
	unsigned char *gettxtad(short id);
	void gotocarte();
	void record();
	char dial_scan(dial_t *dial);
	char dialoscansvmas(dial_t *dial);
	char dialo_even(perso_t *perso);
	void stay_here();
	void mort(short vid);
	void evenchrono();
	void chronoon(short t);
	void prechargephrases(short vid);
	void effet1();
	void effet2();
	void effet3();
	void effet4();
	void ClearScreen();
	void colimacon(short pattern[16]);
	void fadetoblack(int delay);
	void fadetoblack128(int delay);
	void fadefromblack128(int delay);
	void rectanglenoir32();
	void setRS1(short sx, short sy, short ex, short ey);
	void setRD1(short sx, short sy, short ex, short ey);
	void wait(int howlong);
	void effetpix();
	void verifh(void *ptr);
	void openbigfile();
	void closebigfile();
	void loadfile(unsigned short num, void *buffer);
	void shnmfl(unsigned short num);
	int ssndfl(unsigned short num);
	void ConvertIcons(icon_t *icon, int count);
	void ConvertLinks(room_t *room, int count);
	void ConvertMacToPC();
	void loadpermfiles();
	char ReadDataSync(unsigned short num);
	void loadpartoffile(unsigned short num, void *buffer, long pos, long len);
	void Expand_hsq(void *input, void *output);
	void ajouinfo(unsigned char info);
	void unlockinfo();
	void nextinfo();
	void delinfo(unsigned char info);
	void updateinfolist();
	void init_globals();
	void initrect();
	void closesalle();
	void afsalle1(room_t *room);
	void afsalle();
	void aflieu();
	void loadsal(short num);
	void specialoutside();
	void specialout();
	void specialin();
	void animpiece();
	void getdino(room_t *room);
	room_t *getsalle(short loc);
	void initlieu(short roomNum);
	void maj2();
	void majsalle1(short roomNum);
	void maj_salle(unsigned short roomNum);
	void initbuf();
	void freebuf();
	void openwindow();
	void EmergencyExit();
	void edmain();
	void intro();
	char testcdromspeed();
	void entergame();
	void signon(char *s);
	void testPommeQ();
	void FRDevents();
	icon_t *scan_icon_list(short x, short y, short index);
	void update_cursor();
	void mouse();
	void showfilm(char arg1);
	void playhnm(short num);
	void bullehnm();
	void musique();
	void startmusique(unsigned char num);
	void musicspy();
	int loadmusicfile(short num);
	void persovox();
	void endpersovox();
	void fademusicup();
	void fademusica0(short delay);
	object_t *getobjaddr(short id);
	void countobjects();
	void showobjects();
	void winobject(short id);
	void loseobject(short id);
	void lostobject();
	char objecthere(short id);
	void objectmain(short id);
	void getobject(short id);
	void putobject();
	void newobject(short id, short arg2);
	void giveobjectal(short id);
	void giveobject();
	void takeobject();
	void newchampi();
	void newnidv();
	void newnido();
	void newor();
	void gotopanel();
	void noclicpanel();
	void generique();
	void cancel2();
	void testvoice();
	void load();
	void initafterload();
	void save();
	void desktopcolors();
	void panelrestart();
	void reallyquit();
	void confirmer(char mode, char yesId);
	void confirmyes();
	void confirmno();
	void restart();
	void EdenQuit();
	void choixsubtitle();
	void reglervol();
	void changervol();
	void newvol(unsigned char *volptr, short delta);
	void playtape();
	void rewindtape();
	void depcurstape();
	void affcurstape();
	void forwardtape();
	void stoptape();
	void cliccurstape();
	void paneltobuf();
	void cursbuftopanel();
	void langbuftopanel();
	void affpanel();
	void afflangue();
	void affcursvol(short x, short vol1, short vol2);
	void affcurseurs();
	void curseurselect(int itemId);
	void afftoppano();
	void affresult();
	void limitezonecurs(short xmin, short xmax, short ymin, short ymax);
	void PommeQ();
	void habitants(perso_t *perso);
	void suiveurs(perso_t *perso);
	void evenements(perso_t *perso);
	void followme(perso_t *perso);
	void rangermammi(perso_t *perso, room_t *room);
	void perso_ici(short action);
	void setpersohere();
	void faire_suivre(short roomNum);
	void suis_moi5();
	void suis_moi(short index);
	void reste_ici5();
	void reste_ici(short index);
	void eloipart();
	char eloirevientq();
	void eloirevient();
	void incphase1();
	void incphase();
	void phase113();
	void phase130();
	void phase161();
	void phase226();
	void phase257();
	void phase353();
	void phase369();
	void phase371();
	void phase385();
	void phase418();
	void phase433();
	void phase434();
	void phase513();
	void phase514();
	void phase529();
	void phase545();
	void phase561();
	void bigphase1();
	void bigphase();
	void phase16();
	void phase32();
	void phase48();
	void phase64();
	void phase80();
	void phase96();
	void phase112();
	void phase128();
	void phase144();
	void phase160();
	void phase176();
	void phase192();
	void phase208();
	void phase224();
	void phase240();
	void phase256();
	void phase272();
	void phase288();
	void phase304();
	void phase320();
	void phase336();
	void phase352();
	void phase368();
	void phase384();
	void phase400();
	void phase416();
	void phase432();
	void phase448();
	void phase464();
	void phase480();
	void phase496();
	void phase512();
	void phase528();
	void phase544();
	void phase560();
	void savegame(char *name);
	void loadrestart();
	void loadgame(char *name);
	void vavaoffsetout();
	void vavaoffsetin();
	void lieuoffsetout();
	void lieuoffsetin();
	void bandeoffsetout();
	void bandeoffsetin();
	char testcondition(short index);
	unsigned short opera_add(unsigned short v1, unsigned short v2);
	unsigned short opera_sub(unsigned short v1, unsigned short v2);
	unsigned short opera_and(unsigned short v1, unsigned short v2);
	unsigned short opera_or(unsigned short v1, unsigned short v2);
	unsigned short opera_egal(unsigned short v1, unsigned short v2);
	unsigned short opera_petit(unsigned short v1, unsigned short v2);
	unsigned short opera_grand(unsigned short v1, unsigned short v2);
	unsigned short opera_diff(unsigned short v1, unsigned short v2);
	unsigned short opera_petega(unsigned short v1, unsigned short v2);
	unsigned short opera_graega(unsigned short v1, unsigned short v2);
	unsigned short opera_faux(unsigned short v1, unsigned short v2);
	unsigned short operation(unsigned char op, unsigned short v1, unsigned short v2);
	unsigned short cher_valeur();
	void monbreak();
	void ret();
	void make_tabcos();
	void make_matrice_fix();
	void projection_fix(cube_t *cube, int n);
	void init_cube();
	void moteur();
	void affiche_objet(cube_t *cube);
	void NEWcharge_map(int file_id, unsigned char *buffer);
	void NEWcharge_objet_mob(cube_t *cube, int file_id, unsigned char *texptr);
	static int next_val(char **ptr, char *error);
	void selectmap(short num);
	void Eden_dep_and_rot();
	void restoreZDEP();
	void affiche_polygone_mapping(cube_t *cube, cubeface_t *face);
	void trace_ligne_mapping(short r3, short r4, short r5, short r6, short r7, short r8, short r9, short r10, short *lines);
	void affiche_ligne_mapping(short r3, short r4, unsigned char *target, unsigned char *texture);
	short OpenDialog(void *arg1, void *arg2);
	void LostEdenMac_InitPrefs();

private:
	short           old_scroll_pos, scroll_pos;
	short           word_2F514;
	unsigned char   fresqTalk;
	unsigned char   keep01, keep02, keep10, keep11, keep12, keep13, keep21, keep22;
	unsigned char   curs_keepbuf[2500];
	short           curs_keepy, curs_keepx;
	short           torchCursor;
	short           cur_bank_num;
	short           glow_h;
	short           glow_w;
	short           glow_y;
	short           glow_x;
	unsigned char   needPaletteUpdate;
	unsigned char   curs_saved;
	unsigned char   showBlackBars;
	unsigned char   fond_saved;
	unsigned char   *bank_data_ptr;
	color3_t        pal_entry;
	color_t         global_palette[256];    //TODO palette_t
	perso_t         *tyranPtr;
	int             last_anim_frame_num;
	int             cur_anim_frame_num;
	int             last_anim_ticks;
	prect_t         *cur_perso_rect;
	short           num_anim_frames;
	short           max_perso_desc;
	short           num_img_desc;
	unsigned char   restartAnimation;
	unsigned char   animationActive;
	unsigned char   animationDelay;
	unsigned char   animationIndex;
	unsigned char   lastAnimationIndex;

	unsigned char   *dword_30724;
	unsigned char   *dword_30728;   //TODO: rename - something amim-related
	unsigned char   *dword_3072C;   //TODO ditto
	unsigned char   *animationTable;
	unsigned char   imagedesc[512];
	unsigned char   *perso_img_bank_data_ptr;
	unsigned char   savedUnderSubtitles;
	short           num_text_lines;
	unsigned char   phraseBuffer[400];
	unsigned char   *text_ptr;
	unsigned char   phraseIconsBuffer[10];
	unsigned char   phraseCoordsBuffer[22];
	unsigned char   *textoutptr;
	unsigned char   *textout;
	object_t        *currentSpecialObject;
	short           word_30AFC;
	unsigned char   byte_30AFE;

	unsigned char   byte_30B00;
	int             dword_30B04;

	char            lastPhrasesFile;
	char            dialogSkipFlags;

	color3_t        newColor;
	color_t         oldPalette[256];    // TODO palette_t ?
	color_t         newPalette[256];
	rect_t          rect_dst, rect_src;
	void            *voiceSamplesBuffer;    //TODO: sound sample buffer
	file_t          h_bigfile;
	unsigned char   info_list[16];
	unsigned char   needToFade;
	unsigned char   lastMusicNum;
	unsigned char   *main_bank_buf;
	unsigned char   *music_buf;
	unsigned char   *gameLipsync;
	unsigned char   *gamePhrases;
	unsigned char   *gameDialogs;   //TODO: rename to dialogs?
	unsigned char   *gameConditions;
	void            *sal_buf;   //TODO: fixme
	unsigned char   *bank_data_buf;
	icon_t          *gameIcons;
	room_t          *gameRooms;
	pak_t           *bigfile_header;
	unsigned char   *glow_buffer;
	unsigned char   *p_mainview_buf;
	unsigned char   *p_view2_buf;
	unsigned char   *gameFont;  //TODO: rename to font?
	unsigned char   *p_subtitlesview_buf;
	unsigned char   *p_underSubtitlesView_buf;
	global_t        *p_global;
	unsigned short  mouse_y_center, mouse_x_center;
	int             quit_flag3;     //TODO: some obsolete error flag?
	unsigned short  machine_speed;
	unsigned char   quit_flag;

	unsigned char   gameStarted;

	unsigned char   quit_flag2;
	unsigned char   soundAllocated;
	soundchannel_t  *music_channel;
	soundchannel_t  *hnmsound_ch;
	sound_t         *voiceSound;

	view_t          *p_view2;
	view_t          *p_underSubtitlesView;
	view_t          *p_subtitlesview;
	view_t          *p_underBarsView;
	view_t          *p_mainview;
	view_t          *p_hnmview;
	hnm_t           *p_hnmcontext;
	filespec_t      bigfilespec;
	rect_t          underSubtitlesBackupRect, underSubtitlesScreenRect, underBottomBarBackupRect, underBottomBarScreenRect,
	                underTopBarBackupRect, underTopBarScreenRect, rect_31C7A;
	int             demoCurrentTicks;
	int             demoStartTicks;
	int             currentTime;
	short           mouse_y;
	short           mouse_x;
	short           doubled;
	short           curs_x_pan;
	short           inventoryScrollDelay;
	short           curs_y, curs_x;
	short           current_cursor;
	icon_t          *current_spot;
	icon_t          *current_spot2;
	unsigned char   pomme_q;
	unsigned char   keybd_held;
	unsigned char   mouse_held;
	unsigned char   normalCursor;
	unsigned char   *p_hnmview_buf;
	unsigned char   showVideoSubtitle;
	unsigned char   videoCanceled;  //TODO: hnm_canceled
	unsigned char   specialTextMode;
	int             hnm_position;
	int             voiceSamplesSize;   //TODO: perso vox sample data len
	short           mus_vol_right;
	short           mus_vol_left;


	unsigned char   animateTalking;
	unsigned char   personTalking;
	unsigned char   mus_fade_flags;

	char            musicSequencePos;
	unsigned char   musicPlaying;

	unsigned char   *mus_samples_ptr;
	unsigned char   *mus_patterns_ptr;  //TODO: sndblock_t ?
	unsigned char   *mus_sequence_ptr;
	soundgroup_t    *mus_queue_grp;
	short           *pCurrentObjectLocation;
	unsigned char   own_objects[128];
	unsigned char   byte_31D64;

	unsigned char   no_palette;
	unsigned char   gameLoaded;
#define MAX_TAPES 16
	tape_t          tapes[MAX_TAPES];
	unsigned char   confirmMode;
	unsigned char   *cur_slider_value_ptr;
	unsigned char   lastMenuItemIdLo;
	short           lastTapeRoomNum;
	short           cur_slider_x;
	short           cur_slider_y;
	short           destinationRoom;
	short           word_31E7A;

	short           word_378CC; //TODO: set by CLComputer_Init to 0
	short           word_378CE;

};

}

#endif
