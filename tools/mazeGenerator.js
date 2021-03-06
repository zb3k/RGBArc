/**
 * Paste code in: https://editor.p5js.org/
 */
const LCD_WIDTH = 8;
const LCD_HEIGHT = 8;
const APP_MAZE_MAX_SCREENS_X = 4;
const APP_MAZE_MAX_SCREENS_Y = 2;
const APP_MAZE_MAX_PATH_STEPS = 2 * 10;

const APP_MAZE_MAX_WIDTH = LCD_WIDTH * APP_MAZE_MAX_SCREENS_X;
const APP_MAZE_MAX_HEIGHT = LCD_HEIGHT * APP_MAZE_MAX_SCREENS_Y;
const APP_MAZE_MAX_VECTOR = APP_MAZE_MAX_WIDTH * APP_MAZE_MAX_HEIGHT - 1;
const APP_MAZE_WALL = 1;
const APP_MAZE_PATH = 0;
const APP_MAZE_GENERATOR_MAX_STEPS =
  (APP_MAZE_MAX_WIDTH * APP_MAZE_MAX_HEIGHT) / 2 - 2;

const pxSize = 15;
let maze = [];

const vector = (x, y) => ({ x, y });
const mazeVector = (x, y) => x + y * APP_MAZE_MAX_WIDTH;
const getXY = (vector) => ({
  x: vector % APP_MAZE_MAX_WIDTH,
  y: Math.floor(vector / APP_MAZE_MAX_WIDTH),
});

const DIRECTION_RIGHT = 0;
const DIRECTION_DOWN = 1;
const DIRECTION_LEFT = 2;
const DIRECTION_UP = 3;

let generator = {
  vector: mazeVector(APP_MAZE_MAX_WIDTH / 2, APP_MAZE_MAX_HEIGHT / 2),
  steps: 0,
  trackingVector: 0,

  direction: Math.round(Math.random() * 3),
  randomSteps: false,
  findAltPosition: false,
  // randomAltPosition: true,
};

function eachPixel(callback) {
  for (let mVector = 0; mVector <= APP_MAZE_MAX_VECTOR; mVector++) {
    callback(mVector, maze[mVector]);
  }
}

function drawPixel(vector) {
  let { x, y } = getXY(vector);
  rect(x * pxSize + pxSize, y * pxSize + pxSize, pxSize, pxSize);
}

// Render
function render() {
  background(100);

  // Grid
  stroke(50);
  eachPixel((vector) => {
    let { x, y } = getXY(vector);
    if (x > 1)
      line(x * pxSize, pxSize, x * pxSize, APP_MAZE_MAX_HEIGHT * pxSize);
    if (y > 1)
      line(pxSize, y * pxSize, APP_MAZE_MAX_WIDTH * pxSize, y * pxSize);
  });

  noStroke();

  fill(255);
  eachPixel((vector, state) => {
    if (state == APP_MAZE_PATH) drawPixel(vector);
  });
}

function setup() {
  createCanvas(
    APP_MAZE_MAX_WIDTH * pxSize + pxSize,
    APP_MAZE_MAX_HEIGHT * pxSize + pxSize
  );

  // Init matrix
  eachPixel((vector) => {
    maze[vector] = APP_MAZE_WALL;
  });

  maze[generator.vector] = APP_MAZE_PATH;

  frameRate(10);
}

function mazeGeneratorMoveToDir(dir) {
  let { x, y } = getXY(generator.vector);
  let xInc = 0;
  let yInc = 0;

  const isHorizontal = dir % 2 == 0;

  switch (dir) {
    case DIRECTION_RIGHT:
      xInc = 1;
      break;
    case DIRECTION_LEFT:
      xInc = -1;
      break;
    case DIRECTION_DOWN:
      yInc = 1;
      break;
    case DIRECTION_UP:
      yInc = -1;
      break;
  }

  x += xInc;
  y += yInc;
  let mVector = mazeVector(x, y);

  // Out of plane
  if (
    x < 0 ||
    y < 0 ||
    x > APP_MAZE_MAX_WIDTH - 2 ||
    y > APP_MAZE_MAX_HEIGHT - 2
  ) {
    return false;
  }

  x += xInc;
  y += yInc;

  let mVector2 = mazeVector(x, y);
  if (
    x >= 0 &&
    y >= 0 &&
    x <= APP_MAZE_MAX_WIDTH - 2 &&
    y <= APP_MAZE_MAX_HEIGHT - 2
  )
    if (maze[mVector2] == APP_MAZE_PATH) {
      return false;
    }

  maze[mVector] = APP_MAZE_PATH;
  maze[mVector2] = APP_MAZE_PATH;

  generator.direction = dir;
  generator.pathSteps += 2;
  generator.steps += 2;
  generator.vector = mVector2;

  return true;
}

function findAltPosition() {
  this.pathSteps = 0;
  if (!generator.randomSteps) {
    if (generator.steps < APP_MAZE_GENERATOR_MAX_STEPS / 1.5) {
      generator.trackingVector = random(0, APP_MAZE_MAX_VECTOR);
      // generator.trackingVector = Math.round(generator.vector / 2);
    } else {
      generator.randomSteps = true;
      generator.trackingVector = 0;
    }
  }

  for (let y = 0; y < APP_MAZE_MAX_HEIGHT; y += 2) {
    for (let x = 0; x < APP_MAZE_MAX_WIDTH; x += 2) {
      let mVector = mazeVector(x, y);
      let state = maze[mVector];
      if (!state && mVector > generator.trackingVector) {
        generator.trackingVector = mVector;
        generator.vector = mVector;
        return;
      }
    }
  }
}

function nextPosition() {
  let dir = generator.direction;

  if (
    generator.findAltPosition ||
    generator.pathSteps > APP_MAZE_MAX_PATH_STEPS
  ) {
    generator.pathSteps = 0;
    findAltPosition();
  }

  // if (generator.pathSteps > 1) {
  dir = Math.round(random(0, 3)); // HARD
  // }

  let attempts = 3;
  let isMove = false;
  do {
    isMove = mazeGeneratorMoveToDir(dir);
    dir = dir + 1 * (generator.steps % 2 ? -1 : 1);
    //   dir++;
    if (dir > 3) dir = 0;
    if (dir < 0) dir = 3;
  } while (!isMove && attempts--);

  generator.findAltPosition = !isMove;
}

let stop = false;
function draw() {
  if (!stop) {
    if (generator.steps != APP_MAZE_GENERATOR_MAX_STEPS) {
      nextPosition();
    } else {
      stop = true;
      console.log("STOP");
    }
  }
  render();

  fill(200, 0, 0);
  drawPixel(generator.vector);

  fill(0, 200, 0);
  drawPixel(generator.trackingVector);
}
