<template>
    <v-container v-if="this.$route.params.pass === 'x85941'" fluid fill-height>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-card tile class="pa-2" :color="this.status.color">{{this.status.text}}</v-card>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8" style="text-align: center;">
                <h2>{{remainingTime}}</h2>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
        <v-row>
            <v-col cols="2"></v-col>
            <v-col cols="8">
                <v-btn width="100%" class="orange" @click="sendCommand('start')" :disabled="!this.status.startEnabled">Start</v-btn>
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
                <v-text-field
                        label="Time"
                        v-model="timeSecs"
                        type="number"/>
                <v-btn width="100%" @click="updateTime">Set time</v-btn>
            </v-col>
            <v-col cols="2"></v-col>
        </v-row>
    </v-container>
</template>

<script>
    export default {
        name: 'Admin',
        data() {
            return {
                timeSecs: 0
            }
        },
        methods: {
            sendCommand: function (command, data = {}) {
                let json = Object.assign({}, {command: command}, data)
                this.ws.send(JSON.stringify(json));
            },
            updateTime: function () {
                console.log(this.timeSecs)
                this.sendCommand('setTime', {time: this.timeSecs})
            },
            restartWs: function () {
                this.disconnect()
                this.restartConnection()
            }
        },
        computed: {
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
