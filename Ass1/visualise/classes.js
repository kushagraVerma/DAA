function trunc(val,to=10){
    return Math.floor(val*to)/to;
}

class Poly{
    constructor(pts=[]){
        this.pts = pts;
        this.color = color(random(100,255),random(100,255),random(100,255),100);
    }
    addPt(x,y){
        // setFac(x,y);
        this.pts.push(createVector(x,-y));
    }
    addPts(pts=[]){
        for(let pt of pts){
            addPt(pt.x,pt.y);
        }
    }    
    drawPoly(){
        push();
        translate(width/2,height/2);
        strokeWeight(2);
        stroke(255);
        fill(this.color);
        beginShape();
        for(let pt of this.pts){
            vertex(pt.x,pt.y);
        }
        endShape(CLOSE);
        noStroke();
        fill(255,0,0);
        for(let pt of this.pts){
            ellipse(pt.x,pt.y,4,4);
        }
        // textSize(10);
        // fill(255,255,0);
        // for(let pt of this.pts){
        //     text(`${trunc(pt.x)},${-trunc(pt.y)}`,pt.x,pt.y);
        // }
        pop();
    }
    logPoly(){
        let s = `${this.pts.length}\n`;
        for(let pt of this.pts){
            s += `${pt.x} ${-pt.y}\n`;
        }
        console.log(s);
    }
    printPoly(elt){
        let s = `${this.pts.length}\n`;
        for(let pt of this.pts){
            s += `${pt.x} ${-pt.y}\n`;
        }
        return s;
    }
}