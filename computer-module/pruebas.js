// for (let i = 1; i <= 100; i++){
//     let text = i;
//     if (i%3==0 && i%5==0){
//         text += " meridiangroup";
//     }else if(i%3 == 0){
//         text += " meridian";
//     }else if(i%5 == 0){
//         text += " group";
//     }
//     console.log(text);
// }

// console.log("one");
// setTimeout(function(){
//     console.log("two")
// }, 0);
// console.log("three");

const ejm = {
    "type":"FeatureCollection",
    "features":[{
        "type":"Feature",
        "properties":{},
        "geometry":{
            "type":"LineString",
            "coordinates":[
                [-80.85937499999999,-1.0546279422758742],
                [-71.3671875,6.664607562172573],
                [-60.1171875,13.239945499286312]
            ]
        }
    }]
};

const ejm2 = {
    "type":"FeatureCollection",
    "features":[{
        "type":"Feature",
        "properties":{},
        "geometry":{
            "type":"LineString",
            "coordinates":[
                [-80.85937499999999,-1.0546279422758742],
                [-71.3671875,6.664607562172573],
                [-60.1171875,13.239945499286312]
            ]
        }
    }]
};

function compareObjects(obj1, obj2) {
    if (typeof obj1 !== "object" || typeof obj2 !== "object" || obj1 === null || obj2 === null) {
        return obj1 === obj2;
    }
    
    const keys1 = Object.keys(obj1);
    const keys2 = Object.keys(obj2);
    if (keys1.length !== keys2.length) {
        return false;
    }

    for (const key of keys1) {
        if (!compareObjects(obj1[key], obj2[key])) {
            return false;
        }
        console.log(key + " igual");
    }

    return true;
}

console.log(compareObjects(ejm, ejm2));

