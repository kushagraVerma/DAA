function trunc(val,to=100){
    return Math.floor(val*to)/to;
}

class Poly{
    constructor(pts=[]){
        this.pts = pts;
        // for(let pt of pts){
        //     if(!pt.x || !pt.y) console.log(pt);
        // }
        this.color = color(random(100,255),random(100,255),random(100,255),200);
        this.stroke = color(255);
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
    drawPoly(drawtxt,transform={dx:0,dy:0,ds:1}){
        const {dx,dy,ds} = transform;
        push();
        translate(width/2+dx/ds,height/2+dy/ds);
        strokeWeight(2);
        stroke(this.stroke);
        fill(this.color);
        beginShape();
        for(let pt of this.pts){
            vertex(pt.x*ds,pt.y*ds);
        }
        endShape(CLOSE);
        noStroke();
        fill(255,0,0);
        for(let pt of this.pts){
            ellipse(pt.x*ds,pt.y*ds,4,4);
        }
        if(drawtxt){
            textSize(10);
            fill(255,255,0);
            for(let pt of this.pts){
                text(`${trunc(pt.x)},${-trunc(pt.y)}`,pt.x*ds,pt.y*ds);
            }
        }
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