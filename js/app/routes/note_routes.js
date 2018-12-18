module.exports = function (app, db) {
    app.post('/api/images', (req, res) => {
        res.send('Hello POST')
    });
    app.get('/api/images', (req, res) => {
        res.send('Hello GET')
    });
    app.get('/api/images/:id', (req, res) => {
        const id = req.params.id;
        res.send('Hello GET ' + id)

    });
};
