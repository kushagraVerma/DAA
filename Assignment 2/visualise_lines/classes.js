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

class ptsNline{
    constructor(pts,a,b){
        this.pts = pts;
        // this.a = a;
        // this.b = b;
        let minX1 = 1e6;
        let maxX1 = -1e6;
        for(let pt of this.pts){
            if(pt.x < minX1){
                minX1 = pt.x;
            }
            if(pt.x > maxX1){
                maxX1 = pt.x;
            }
        }
        function y(x){
            return (a*x+b);
        }
        // console.log(y);
        // this.ln = {x1: minX1, y1: y(minX1), x2: maxX1, y2: y(maxX1)};
        this.ln = {x1: -1e6, y1: y(-1e6), x2: 1e6, y2: y(1e6)};
        this.col = color(random(100,255),random(100,255),random(100,255));
    }
    drawLn(transform={dx:0,dy:0,ds:1}){
        const {dx,dy,ds} = transform;
        push();
        translate(width/2+dx/ds,height/2+dy/ds);
        strokeWeight(2);
        stroke(this.col);
        let x1,y1,x2,y2 = this.ln;
        // console.log(typeof);
        ellipse(x1*ds,y1*ds,5,100);
        // line(-100*ds,83*ds,-96*ds,14*ds);
        // console.log(this.ln);
        line(x1*ds,y1*ds,x2*ds,y2*ds);
        // beginShape(LINES);
        // vertex(x1,-y1);
        // vertex(x2,-y2);
        // endShape();
        pop();
    }
    drawPts(drawtxt,transform={dx:0,dy:0,ds:1}){
        const {dx,dy,ds} = transform;
        push();
        translate(width/2+dx/ds,height/2+dy/ds);
        // strokeWeight(2);
        // stroke(this.stroke);
        // fill(this.color);
        // beginShape();
        // for(let pt of this.pts){
        //     vertex(pt.x*ds,pt.y*ds);
        // }
        // endShape(CLOSE);
        // stroke(this.col);
        // strokeWeight(2);
        // stroke(255);
        // line(-100,83,-96,14);
        // let x1,y1,x2,y2 = this.ln;
        // console.log(this.ln);
        // line(x1,y1,x2,y2);
        // debugger;
        noStroke();
        fill(255,0,0);
        // fill(this.col);
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
}
