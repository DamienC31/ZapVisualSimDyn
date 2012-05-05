class render {
	public:
		render(world*);
		void drawLines();
		void drawGrid();
		void drawToolBox();
		void drawStructures();


		//Color stuff
		GLuint setupColor(float,float,float);
		void setZoneColor(int);
	private:
		world* parent;
		cell** map;
		UInterface* UI;
		GLuint nozoneColor, residColor, commColor, indusColor;

};
