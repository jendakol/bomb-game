import axios from "axios";

let GlobalFunctions = {
    mounted: function () {
        this.$vuetify.theme.dark = true
    },
    data() {
        return {
            host: process.env.NODE_ENV === 'development' ? "192.168.1.5" : window.location.hostname,
        }
    },
    methods: {
        ajaxGet(path, data, timeout) {
            return axios.get("http://" + this.host + "/" + path, {timeout: timeout === undefined ? 5000 : timeout})
                .then(t => {
                    return t.data;
                }).catch(err => {
                    console.log(err);
                    this.$snotify.error("Error", err, {
                        closeOnClick: true,
                        timeout: 5000
                    })
                })
        },
        ajax(path, data, timeout) {
            return axios.post("http://" + this.host + "/" + path, data, {timeout: timeout === undefined ? 5000 : timeout})
                .then(t => {
                    return t.data;
                }).catch(err => {
                    console.log(err);
                    this.$snotify.error("Error", err, {
                        closeOnClick: true,
                        timeout: 5000
                    })
                })
        },
        initWs: function () {
            let hostname = this.host

            console.log("Connecting to ws://" + hostname + "/ws")
            this.ws = new WebSocket("ws://" + hostname + "/ws")

            this.ws.onopen = () => {
                this.connected = true
            };

            this.ws.onerror = (err) => {
                this.connected = false;
                console.log("WS error: " + JSON.stringify(err));
                this.restartConnection()
            };

            this.ws.onmessage = (data) => {
                this.bombState = JSON.parse(data.data);
            };

            this.ws.onclose = () => {
                if (this.connected) {
                    console.log("Connection closed, restarting");
                    this.restartConnection()
                } else {
                    console.log("Connection closed")
                }
            };

            window.addEventListener('beforeunload', () => {
                console.log("Closing the page")
                if (this.ws != null && this.ws.readyState === 1) {
                    this.disconnect()
                }
            }, false)
        },
        restartConnection: function () {
            setTimeout(() => {
                this.initWs()
            }, 1000)
        },
        disconnect: function () {
            console.log("Closing connection")
            this.connected = false

            if (this.ws != null)
                this.ws.close()
        },
    },
    computed: {
        ws: {
            get: function () {
                return this.$store.getters.ws
            },
            set: function (value) {
                this.$store.commit('initWs', value)
            }
        },
        connected: {
            get: function () {
                return this.$store.getters.connected
            },
            set: function (value) {
                this.$store.commit('setConnected', value)
            }
        },
        isManualSelected: {
            get: function () {
                return this.$store.getters.manualSelected
            },
            set: function (value) {
                this.$store.commit('manualSelect', value)
            }
        },
        bombState: {
            get: function () {
                return this.$store.getters.bombState
            },
            set: function (value) {
                this.$store.commit('setBombState', value)
            }
        },
        puzzleModules: {
            get: function () {
                return this.$store.getters.puzzles
            },
            set: function (value) {
                this.$store.commit('setPuzzles', value)
            }
        },
        remainingTime: {
            get: function () {
                let secs = this.$store.getters.bombState.remainingSecs;
                if (secs > 0) {
                    let mins = Math.floor(secs / 60);
                    secs = secs % 60;
                    return "00:" + mins.toString().padStart(2, "0") + ":" + secs.toString().padStart(2, "0")
                } else {
                    return "--:--";
                }
            }
        },
    }
}

export default GlobalFunctions
