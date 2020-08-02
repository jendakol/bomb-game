const path = require('path');
const webpack = require('webpack');

const UglifyJsPlugin = require('terser-webpack-plugin');
const OptimizeCSSAssetsPlugin = require('optimize-css-assets-webpack-plugin');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
    "transpileDependencies": [
        "vuetify"
    ],
    configureWebpack: config => {
        config.optimization = {
            runtimeChunk: 'single',
            minimizer: [
                new UglifyJsPlugin({
                    cache: true,
                    parallel: true,
                    sourceMap: !isProd
                }),
                new OptimizeCSSAssetsPlugin({
                    cssProcessorPluginOptions: {
                        preset: ['default', {discardComments: {removeAll: true}}],
                    }
                }),
            ]
        };
        config.output = {
            path: path.resolve(__dirname, 'dist'),
            publicPath: '/',
            filename: 'js/[hash:5].js',
            chunkFilename: 'js/[id].[hash:5].js'
        };
        config.plugins.concat([
            new webpack.HashedModuleIdsPlugin(),
            new webpack.optimize.AggressiveMergingPlugin(),
        ]);
    }
}
