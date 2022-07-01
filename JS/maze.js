class Maze{

    constructor(width, height){
        this.width = width; 
        this.height = height;
        this.cells = [];
    }

    initialise(){
        for (let x = 0; x < this.height; x++){

            this.row = [];

            for (let y = 0; y < this.width; y++){
                let cell = new Cell(' ');
                this.row.push(cell);
            }

            this.cells.push(this.row);

        }
    }

    generate(){
        let x = 1, y = 1;

        this.cells[x][y].in = true;

        let success;

        do {
            
        } while (!success);
    }

}

class Cell{
    constructor(type){
        this.type = type;
        this.in = false;
        this.up = false;
        this.left = false;
    }

    is_path(){
        return this.type == ' ';
    }

    is_wall(){
        return this.type == '#';
    }

    is_visited(){
        return this.type == '.';
    }
    
}