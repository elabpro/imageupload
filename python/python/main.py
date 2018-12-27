# ImageUpload API on Python
# 2018 (c) Eugene Arbatsky

from flask import Flask
from flask import Response
from flask import jsonify
from flask import request
from flask import send_file
import json
import os
import redis
import urllib

app = Flask(__name__)

imagesDir = "images.full"
imagesThumbDir = "images.thumb"

# GET
# /api/images
@app.route('/api/images', methods=['GET'])
def getImages():
    files = os.listdir(imagesDir)
    return jsonify(files)

# GET
# /api/images/{id}
@app.route('/api/images/<int:imageId>', methods=['GET'])
def getImage(imageId):
    return send_file(imagesDir + "/" + str(imageId), mimetype='image/png')

# POST
# /api/images
@app.route('/api/images', methods=['POST'])
def postImage():
    files = []
    if request.content_type == 'application/json':
        content = request.json
        for rec in content:
            try:
                files.append(download(rec["url"]))
            except KeyError:
                print "No url"

            try:
                files.append(uploadContent(rec["content"]))
            except KeyError:
                print "No content"
    else:
        try:
            url = request.form.get("url")
            if url is not None and len(url) > 0:
                files.append(download(url))
        except KeyError:
            print "No URL"
        for fileInUpload in request.files:
            file = request.files[fileInUpload]
            fileId = getCounter()
            filename = getFilename(fileId)
            print "Saving file " + file.filename + " to " + filename
            file.save(filename)
            files.append(fileId)
    return json.dumps(files)

# Download from from URL
#
# @param string url
# @return int fileId
def download(url):
    fileId = getCounter()
    filename = getFilename(fileId)
    print "Downloading " + url + " to " + filename
    createLock(fileId)
    urllib.urlretrieve(url, filename)
    deleteLock(fileId)
    return fileId

# Upload file from BASE64 content
#
# @param string
# @return int fileId
def uploadContent(content):
    fileId = getCounter()
    filename = getFilename(fileId)
    print "Uploading from content to " + filename
    createLock(fileId)
    fh = open(filename, "wb")
    fh.write(content.decode('base64'))
    fh.close()
    deleteLock(fileId)
    return fileId

# Get ID from redis
# @return int fileId
def getCounter():
    r = redis.Redis(host='localhost', port=6379, db=0)
    return r.incr("counter")

# Get filename for an image with specified ID
#
# @param int fileId
def getFilename(fileId):
    return "images.full/" + str(fileId)

# Create lock for an image with specified ID
#
# @param int fileId
def createLock(fileId):
    fh = open(getFilename(fileId) + ".lock", "wb")
    fh.write("1")
    fh.close()

# Delete lock for an image with specified ID
#
# @param int fileId
def deleteLock(fileId):
    os.remove(getFilename(fileId) + ".lock")

if __name__ == '__main__':
    port = 8080
    print("Pythin project v.0.1. Image API is ready")
    print("URL http://localhost:" + str(port) + "/api/images")
    print("Source code from https://github.com/elabpro/imageupload")
    app.run(host='localhost', port=port, debug=True)
