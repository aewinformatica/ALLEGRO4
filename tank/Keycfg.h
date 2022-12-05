
void		InitJoyStick(void);

bool		IsKeyUp(int kbutton);
bool		IsKeyDn(int kbutton);

bool		IsJoyButtonDn(int j, int b);
bool		IsJoyButtonUp(int j, int b);

bool		IsJoyStickMoved(int j, int s, int a, int sensitivity);

extern	bool		usejoystick;
extern	int			nojoytime;

extern	bool		usekeyboard;
void		CheckKeys(void);
void		ClearAllKeys(void);
void		CheckJoyStick(void);
void		mouse_callback2(int flags);
void		Input_Thinker(void);
