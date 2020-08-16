<template>
    <v-container v-if="this.$route.params.pass === 'x85941'" fluid fill-height>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-card tile class="pa-2" :color="this.status.color">{{ this.status.text }}</v-card>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8" style="text-align: center;">
                <h2>{{ remainingTime }}</h2>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-btn width="100%" class="orange" @click="timeDialog.shown = true" :disabled="!this.status.startEnabled">Start</v-btn>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-btn width="100%" class="green" @click="sendCommand('defuse')" :disabled="!this.status.defuseEnabled">Defuse</v-btn>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-btn width="100%" class="red" @click="sendCommand('explode')" :disabled="!this.status.explodeEnabled">Explode</v-btn>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-btn width="100%" @click="this.restartWs">Restart connection</v-btn>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-subheader>Keyboard module</v-subheader>
                <v-slider
                    :key="timeSecs"
                    v-model="keyboardSlider"
                    :thumb-size="20"
                    ticks="always"
                    tick-size="4"
                    thumb-label="always"
                    class="align-center"
                    hide-details
                    min="0"
                    :max="keyboardMax"
                />
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-subheader>Cable module</v-subheader>
                <v-slider
                    :key="timeSecs"
                    v-model="cableSlider"
                    :thumb-size="20"
                    ticks="always"
                    tick-size="4"
                    thumb-label="always"
                    class="align-center"
                    hide-details
                    min="0"
                    :max="cableMax"
                />
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>

        <v-dialog v-model="timeDialog.shown"
                  width="290px"
        >
            <v-time-picker v-model="timeDialog.value"
                           format="24hr"
                           :use-seconds="true"
                           :full-width="true"
            >
                <v-spacer></v-spacer>
                <v-btn text color="primary" @click="start">OK</v-btn>
            </v-time-picker>
        </v-dialog>
    </v-container>
</template>

<script>
export default {
    name: 'Admin',
    data() {
        return {
            modal: false,
            timeSecs: 0,
            timeDialog: {
                shown: false,
                value: this.remainingTime,
            },
            cableMax: 10,
            keyboardMax: 10
        }
    },
    mounted() {
        this.downloadPuzzles()
    },
    methods: {
        start: function () {
            this.timeDialog.shown = false

            let parsedTime = this.timeDialog.value.split(':')
            this.timeSecs = (parseInt(parsedTime[0]) * 3600) + (parseInt(parsedTime[1]) * 60) + parseInt(parsedTime[2])
            // console.log(this.timeSecs)
            this.sendCommand('start', {time: this.timeSecs})
        },
        sendCommand: function (command, data = {}) {
            let json = Object.assign({}, {command: command}, data)
            this.ws.send(JSON.stringify(json))
        },
        restartWs: function () {
            this.disconnect()
            this.restartConnection()
        },
        updateKeyboardProgress: function (value) {
            this.sendCommand('progress', {module: 0, value: value});
        },
        updateCableProgress: function (value) {
            this.sendCommand('progress', {module: 1, value: value});
        },
        downloadPuzzles: function () {
            this.ajaxGet("puzzles.json", {}, 15000).then(r => {
                this.puzzleModules = r;
                console.log(r)
                this.cableMax = r.cables.puzzles.length
                this.keyboardMax = r.keyboard.puzzles.length
            })

        }
    },
    computed: {
        keyboardSlider: {
            get: function () {
                return this.bombState.progress0
            },
            set: function (value) {
                this.updateKeyboardProgress(value)
            }
        },
        cableSlider: {
            get: function () {
                return this.bombState.progress1
            },
            set: function (value) {
                this.updateCableProgress(value)
            }
        },
        status: {
            get: function () {
                switch (this.bombState.status) {
                    case 0:
                        return {color: "gray", text: "Started", startEnabled: true, defuseEnabled: false, explodeEnabled: false}
                    case 1:
                        return {color: "orange", text: "Running", startEnabled: false, defuseEnabled: true, explodeEnabled: true}
                    case 2:
                        return {color: "green", text: "Defused", startEnabled: true, defuseEnabled: false, explodeEnabled: false}
                    case 3:
                        return {color: "red", text: "Exploded", startEnabled: true, defuseEnabled: false, explodeEnabled: false}
                }

                return {color: "red", text: "Exploded", startEnabled: true}
            }
        }
    }
}
</script>
