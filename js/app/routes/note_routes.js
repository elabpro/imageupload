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

/**
 * Create file from base64 encoded content
 *
 * @param string content
 * @returns int
 */
function createImageFromContent(content) {
    var fileId = getCounter();
    filename = getFilename(fileId);
    console.log("Saving content to " + filename);
    require("fs").writeFile(filename, content, 'base64', function (err) {
        console.log(err);
    });
    return fileId;
}

/**
 * Create file from uploaded file
 * @param tmpFilename
 * @return
 */
function createImageFromFile() {

}

/**
 * Download and save image to a file
 *
 * @param string url
 * @returns int
 */
function downloadFile(url) {
    var fileId = getCounter();
    filename = getFilename(fileId);
    console.log("Downloading " + url + " to file " + filename);
    var file = fs.createWriteStream(filename);
    var request = http.get(record.url, function (response) {
        response.pipe(file);
    });
    return fileId;
}

module.exports = function (app, db) {
    /**
     * Method POST
     * Upload images in PNG. Body:
     * - multipart/encoded files
     * - JSON {{content=#base64_image_content#|url=#url#}}
     * - url=#url#
     *
     * @usage http://localhost/api/images
     *
     * @param req
     * @param res
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
                        fileId = downloadFile(record.url);
                        filesId.push(fileId);
                    }
                    if (record.content != null) {
                        fileId = createImageFromContent(record.content);
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
     * Method GET Getting array of images (JSON)
     * On return sends JSON array of filenames
     *
     * @usage http://localhost:8080/api/images
     *
     * @param req
     * @param res
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
     * Method GET Getting an image
     * On return sends binary image
     *
     * @usage http://localhost:8080/api/images/{id}
     *
     * @param req
     * @param res
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
