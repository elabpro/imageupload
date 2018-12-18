const imagesDir = "images.full";
const imagesThumbDir = "images.thumb";

const fs = require('fs');
const http = require('http');

function getCounter() {
    return 2;
}

function getFilename(id) {
    return imagesDir + "/" + id;
}

module.exports = function (app, db) {
    /**
     * 
     */
    app.post('/api/images', (req, res) => {
        if (req.header("Content-Type") == "application/json") {
            console.log("JSON");
            var jsonString = '';

            req.on('data', function (data) {
                jsonString += data;
            });

            req.on('end', function () {
                res.contentType('application/json');
                filesId = [];
                records = JSON.parse(jsonString);
                records.forEach(record => {
                    if (record.url != null) {
                        fileId = getCounter();
                        filename = getFilename(fileId);
                        console.log("Downloading " + record.url + " to file " + filename);
                        var file = fs.createWriteStream(filename);
                        var request = http.get(record.url, function (response) {
                            response.pipe(file);
                        });
                        filesId.push(fileId);
                    }
                    if (record.content != null) {
                        fileId = getCounter();
                        filename = getFilename(fileId);
                        console.log("Saving content to " + filename);
                        require("fs").writeFile(filename, record.content, 'base64', function (err) {
                            console.log(err);
                        });
                        filesId.push(fileId);
                    }
                });
                res.send(JSON.stringify(filesId));
            });

        } else {
            console.log("MP");
            res.send('Hello POST')
        }
    });
    /**
     * 
     */
    app.get('/api/images', (req, res) => {
        fs.readdir(imagesDir, (err, files) => {
            filesId = [];
            files.forEach(file => {
                filesId.push(file);
            });
            res.contentType('application/json');
            res.send(JSON.stringify(filesId));
        })

    });
    /**
     * 
     */
    app.get('/api/images/:id', (req, res) => {
        const id = req.params.id;
        filename = getFilename(id);
        fs.exists(filename, function (exists) {
            if (exists) {
                res.writeHead(200, {
                    "Content-Type": "image/png",
                    "Content-Disposition": "attachment; filename=" + id + ".png"
                });
                fs.createReadStream(filename).pipe(res);
            } else {
                res.writeHead(400, {"Content-Type": "text/plain"});
                res.end("ERROR File does not exist");
            }
        });
    });
};
