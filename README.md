# AffineX

**AffineX** is a minimal PS1-style 3D renderer built on modern OpenGL.  
It intentionally embraces affine texture mapping, low precision, and fixed-function-era constraints to expose how real-time 3D graphics actually work.

This project is not about realism.  
It is about understanding the graphics pipeline from the ground up.

---

## Why AffineX?

Modern renderers hide a lot of complexity behind abstractions.  
AffineX does the opposite.

By recreating PlayStation-era limitations using programmable shaders, AffineX makes the following concepts visible and intuitive:

- How vertices move through the pipeline
- Why perspective-correct texturing exists
- How lighting works without modern tricks
- How visual artifacts emerge from math

If something looks “wrong”, that is usually intentional.

---

## Visual Style

AffineX targets a **PS1-era aesthetic**:

- Low-poly geometry
- Affine (non-perspective-correct) texture mapping
- Nearest-neighbor texture filtering
- Vertex-level lighting
- Visible jitter and texture warping

Artifacts are features.

---

## Scope

AffineX is intentionally limited.

### Included
- Forward rendering
- Triangle-based meshes
- Single camera
- Single directional light
- Per-vertex diffuse lighting
- Low-resolution textures

### Not Included
- PBR
- Shadows
- Post-processing
- Deferred rendering
- Animation or skinning
- Editor tools

The goal is clarity, not completeness.

---

## Tech

- **Language:** C++
- **Graphics API:** OpenGL
- **Shaders:** GLSL
- **Target:** Real-time rendering

Modern OpenGL is used as a tool, not as a feature set.

---

## Learning Goals

This project is meant to build intuition around:

- Coordinate spaces (model → world → view → clip)
- Vertex vs fragment responsibilities
- Depth and precision problems
- GPU data flow
- Fixed-function mindset in a programmable pipeline

AffineX is a learning artifact before it is a renderer.

---

## Status

🚧 **Work in progress**

Current focus:
- Core rendering pipeline
- Affine texture mapping
- Basic lighting

---

## Philosophy

> If you understand why it looks bad, you understand graphics.

---

## License

MIT
