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
        <v-row justify="space-around">
            <v-col cols="8">
              <v-dialog
                  ref="dialog"
                  persistent
                  width="290px"
              >
                  <template v-slot:activator="{ on, attrs }">
                    <v-text-field
                        v-model="timer"
                        label="Time"
                        readonly
                        v-bind="attrs"
                        v-on="on"
                    ></v-text-field>
                  </template>
                  <v-time-picker v-model="timer"
                                 format="24hr"
                                 :use-seconds="true"
                                 :full-width="true"
                                 v-on:change="updateTime"
                  >
                  <v-spacer></v-spacer>
                  <v-btn text color="primary" @click="$refs.dialog.save(timer)">OK</v-btn>
                  </v-time-picker>
              </v-dialog>
            </v-col>
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
                v-on:click="updateKeyboardProgress"
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
                :value="this.bombState.progress0"
                v-model="cableSlider"
                :thumb-size="20"
                ticks="always"
                tick-size="4"
                thumb-label="always"
                class="align-center"
                hide-details
                min="0"
                :max="cableMax"
                v-on:click="updateCableProgress"
            />
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
                modal: false,
                timeSecs: 0,
                timer: null,
                keyboardSlider: null,
                cableSlider: null,
                cableMax: 10,
                keyboardMax: 10
            }
        },
      mounted() {
          this.downloadPuzzles();
      },
      methods: {
            sendCommand: function (command, data = {}) {
                let json = Object.assign({}, {command: command}, data)
                this.ws.send(JSON.stringify(json));
            },
            updateTime: function () {
                let parsedTime = this.timer.split(':');
                this.timeSecs = (parseInt(parsedTime[0]) * 3600) + (parseInt(parsedTime[1]) * 60) + parseInt(parsedTime[2]);
                // console.log(this.timeSecs);
                this.sendCommand('setTime', {time: this.timeSecs})
            },
            restartWs: function () {
                this.disconnect()
                this.restartConnection()
            },
            updateSliders: function () {
                this.keyboardSlider = this.bombState.progress0
                this.cableSlider = this.bombState.progress1
            },
            updateKeyboardProgress: function () {
                console.log(this.keyboardSlider);
                this.sendCommand('progress', {module: 0, value: this.keyboardSlider});
            },
            updateCableProgress: function () {
                this.sendCommand('progress', {module: 1, value: this.cableSlider});
            },
            downloadPuzzles: function () {
                this.ajaxGet("puzzles.json", {}, 15000).then(r => {
                  this.puzzleModules = r;
                  this.cableMax = r.cables.puzzles.length
                  this.keyboardMax = r.keyboard.puzzles.length
                })

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
