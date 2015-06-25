#ifndef COMMENT_H
#define COMMENT_H

class Comment {
private:
	string text;
	int nlines, maxw;
	Entity *parent;
	bool right;
	int target_y;
	int real_y;
	int height;
public:
	void Tick();
	void Draw();
	void Reflow();
	Comment(Entity *parent, string text);
	~Comment();
};

#endif

