import Vue from "vue";
import Vuex from 'vuex'

Vue.use(Vuex)

const StateStore = new Vuex.Store({
    state: {
        manualSelected: false,
        connected: false,
        ws: null,
        bombState: {},
        puzzles: {}
    },
    getters: {
        manualSelected: function (state) {
            return state.manualSelected
        },
        ws: function (state) {
            return state.ws
        },
        connected: function (state) {
            return state.connected
        },
        bombState: function (state) {
            return state.bombState
        },
        puzzles: function (state) {
            return state.puzzles
        },
    },
    mutations: {
        manualSelect(state, value) {
            state.manualSelected = value
        },
        initWs(state, ws) {
            state.ws = ws
        },
        setConnected(state, connected) {
            state.connected = connected
        },
        setBombState(state, value) {
            state.bombState = value
        },
        setPuzzles(state, value) {
            state.puzzles = value
        },
    }
})

export default StateStore
